#include <common.hpp>
#include <pthread.h>
#include  <sys/epoll.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

static int g_ep_listen_fd;
static int g_ep_read_fd;
static pthread_t g_athread_id[MAX_ACCEPT_THREADS];
static pthread_t g_rthread_id[MAX_READER_THREADS];
static pthread_t g_wthread_id[MAX_WORKER_THREADS];

void close_socket(int sd)
{
	//printf("Closing socket %d\n", sd);
	if (sd > 2) close(sd);
}

int create_epoll(int size)
{
	int ep_fd;
	if( -1 == (ep_fd = epoll_create(size))) {
		ERROR("Failed to create epoll");
	}
	return ep_fd;
}

/* Below funciton is blocking  */
int wait_epoll(const char *str, int ep_fd, struct epoll_event *ep_events)
{
	int nfds = 0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	memset(ep_events, 0, sizeof(struct epoll_event));
	for(;;) { // This loop is because GDB sends EINTR signal
		if(-1 == (nfds = epoll_wait(ep_fd, ep_events, MAX_EVENTS, -1))) {
			if(errno == EINTR)
				continue;
			else {
				ERROR("[%s] Failed to wait on epoll ep_fd=%d (%s)", str, ep_fd, strerror(errno));
			}
		}
		break;
	}
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	return nfds;
}

void add_to_epoll(const char *str, int ep_fd, int sd, uint32_t events)
{
	struct epoll_event ep_event;
	memset(&ep_event, 0, sizeof(ep_event));
	ep_event.data.fd  = sd;
	ep_event.events   = events;

	if(-1 == epoll_ctl(ep_fd, EPOLL_CTL_ADD, sd, &ep_event)) {
		ERROR("[%s] Failed to epoll control add sd=%d to ep_fd=%d", str,sd, ep_fd);
	}
	//PRINTF("[%s] epoll add sd=%d to ep_fd=%d \n", str, sd, ep_fd);
}

void delete_from_epoll(const char *str, int ep_fd, int sd)
{
	struct epoll_event ep_event;

	if(-1 == epoll_ctl(ep_fd, EPOLL_CTL_DEL, sd, &ep_event)) {
		ERROR("[%s] Failed to epoll control delete sd=%d from ep_fd=%d (%s)\n",
			str, sd, ep_fd, strerror(errno));
	}
	//PRINTF("[%s] epoll delete sd=%d from ep_fd=%d \n", str, sd, ep_fd);
}

void allocate_epoll_event(struct epoll_event **ep_events)
{
	struct epoll_event *ptr;
	if(NULL == (ptr=(struct epoll_event *) malloc (sizeof(struct epoll_event) * MAX_EVENTS))) {
		ERROR("Failed to malloc for epoll event struct (%d)", MAX_EVENTS);
	}
	*ep_events = ptr;
}

void deallocate_epoll_event(void *ptr)
{
	free(ptr);
}

void *accept_socket_thread(void *arg)
{
	int i;
	int listen_socket   = *((int *) arg);
	socklen_t client_size = sizeof(struct sockaddr_in);
	struct sockaddr_in client_addr;
	struct epoll_event *ep_events = NULL;
	allocate_epoll_event(&ep_events);
	pthread_cleanup_push(deallocate_epoll_event, ep_events);

	for(;;) {
		//PRINTF("%s: [%x] Waiting \n", __FUNCTION__, ep_events);
		int nfds = wait_epoll(__FUNCTION__, g_ep_listen_fd, ep_events);
		//PRINTF("%s: [%x] Has come out with nfds=%d \n", __FUNCTION__, ep_events, nfds);
		for(i=0; i<nfds; ++i) {
			if ((ep_events[i].events & EPOLLERR) ||
				(ep_events[i].events & EPOLLHUP) ||
				(!(ep_events[i].events & EPOLLIN)))  {
					ERROR("epoll-1 (%s)",  strerror(errno));
					close (ep_events[i].data.fd);
					continue;
			} else if(ep_events[i].data.fd == listen_socket) {
				int sd;
				if(-1 == (sd = accept(listen_socket, (struct sockaddr *)&client_addr,
					(socklen_t *)&client_size))) {
					ERROR("Failed to accept connection (%s)", strerror(errno));
					continue;
				}
				//PRINTF("New connection accepted sd=%d \n", sd);
				add_to_epoll(__FUNCTION__, g_ep_read_fd, sd, EPOLLIN | EPOLLET | EPOLLONESHOT); // Enables edge triggered behaviour
			} else
				ERROR("DON'T KNOW WHAT TO DO HERE (Error: listen socket is wrong)");
		}
	}
	pthread_cleanup_pop(1);
	return NULL;
}

int read_query_from_socket(int sd, circular_queue_t *queue)
{
	int num_bytes 	= 0;
	char buff[MAX_PACKET_LENGTH];
	job_t *job 		= (job_t *) malloc(sizeof(job_t));
	job->sd 		= sd;
	job->buff 		= NULL;
	std::string rcv_data;
	while (1) {
		if (-1 == (num_bytes=recv(sd, buff, MAX_PACKET_LENGTH-1, 0))) {
			printf("ERROR: Failed to receive data from client\n");
			break;
		} else if (num_bytes == 0) {
			break;
		} else {
			buff[num_bytes] = '\0';
			char *ptr = strstr(buff, END_OF_DATA);
			if (ptr) {
				*ptr = '\0';
				rcv_data +=  string(buff);
				break;
			} else rcv_data +=  string(buff);
		}
	}
	//TRACE("[%s] [%d]", rcv_data.c_str(), num_bytes);
	job->buff	= (char *) malloc(rcv_data.size() + 1);
	sprintf(job->buff, "%s", rcv_data.c_str());
	insert_to_queue(job, queue);
	return num_bytes;
}

void *read_socket_thread(void *arg)
{
	circular_queue_t *queue = (circular_queue_t *)arg;
	int i;
	struct epoll_event *ep_events = NULL;
	allocate_epoll_event(&ep_events);
	pthread_cleanup_push(deallocate_epoll_event, ep_events);
	for(;;) {
		//PRINTF("[%x] Waiting \n",  ep_events);
		int nfds = wait_epoll(__FUNCTION__, g_ep_read_fd, ep_events);
		for(i=0; i<nfds; ++i) {
			if ((ep_events[i].events & EPOLLERR) ||
				(ep_events[i].events & EPOLLHUP) ||
				(!(ep_events[i].events & EPOLLIN)))  {
					ERROR("epoll-2 (%s)\n",  strerror(errno));
					close(ep_events[i].data.fd);
					continue;
			} else if(!read_query_from_socket(ep_events[i].data.fd, queue)) {
				TRACE("[%x] Reader thread - Client is dead sd=%d\n", ep_events, ep_events[i].data.fd);
				close_socket(ep_events[i].data.fd);
			}
		}
	}
	pthread_cleanup_pop(1);
	return NULL;
}

void excute_job(job_t *job)
{
	PRINTF ("[threadid: %x][sd:%d] Sleeping for half second\n", (unsigned)pthread_self(), job->sd);
	sleep(1/2);
	send(job->sd, "ACK", 3, 0);
}

void *process_query_thread(void *arg)
{
	circular_queue_t *queue = (circular_queue_t *)arg;

	while(1) {
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		//PRINTF("%s: Thread waiting for job\n", __FUNCTION__);
		job_t *job = delete_from_queue(queue);
		if(job) {
			string username;
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			recreate_log_file();
			PRINTF("Rcvd job %d : [%s]\n", job->sd, job->buff);
			excute_job(job);
			free(job->buff);
			close(job->sd);
			free(job);
		}
	}
	return NULL;
}

void thread_clean_up()
{
	unsigned int loop = 0;
	for(loop=0; loop<MAX_ACCEPT_THREADS; loop++)
		pthread_cancel(g_athread_id[loop]);
	for(loop=0; loop<MAX_READER_THREADS; loop++)
		pthread_cancel(g_rthread_id[loop]);
	for(loop=0; loop<MAX_WORKER_THREADS; loop++)
		pthread_cancel(g_wthread_id[loop]);
}


void create_threads(int listen_socket, circular_queue_t *queue)
{
	unsigned int loop = 0;
	void *process_query_thread(void *arg);

	// Watch list socket
	g_ep_listen_fd = create_epoll(1);

	// Client fds watch
	g_ep_read_fd   = create_epoll(MAX_CONNECTION);
	add_to_epoll(__FUNCTION__, g_ep_listen_fd, listen_socket, EPOLLIN); // Enables level triggered behaviour

	for(loop=0; loop<MAX_ACCEPT_THREADS; ++loop)
		pthread_create(&g_athread_id[loop], NULL, accept_socket_thread, &listen_socket);

	for(loop=0; loop<MAX_READER_THREADS; ++loop)
		pthread_create(&g_rthread_id[loop], NULL, read_socket_thread, queue);

	for(loop=0; loop<MAX_WORKER_THREADS; ++loop)
		pthread_create(&g_wthread_id[loop], NULL, process_query_thread, queue);

	void *exit_status;

	for(loop=0; loop<MAX_WORKER_THREADS; loop++)
		pthread_join(g_wthread_id[loop], &exit_status);

	for(loop=0; loop<MAX_READER_THREADS; loop++)
		pthread_join(g_rthread_id[loop], &exit_status);

	for(loop=0; loop<MAX_ACCEPT_THREADS; loop++)
		pthread_join(g_athread_id[loop], &exit_status);

	delete_from_epoll(__FUNCTION__, g_ep_listen_fd, listen_socket);
	close(g_ep_listen_fd);
	close(g_ep_read_fd);
	thread_clean_up();
}


int create_server_socket()
{
	/* PF_INET      - IPv4 Internet protocols
	 * SOCK_STREAM  - TCP
	*/
	int listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == listen_socket) {
		ERROR_EXIT("Failed to create server socket [%s]\n", strerror(errno));
	}
	return listen_socket;
}

void  set_socket_options(int listen_socket, int opt_name, const char *opt_val, socklen_t opt_len)
{
	if (-1 == setsockopt(listen_socket, SOL_SOCKET, opt_name, opt_val, opt_len)) {
		ERROR_EXIT("Failed to set socket options [%s]\n", strerror(errno));
	}
}

void get_ip_address(struct hostent *host, char *ip_address)
{
	struct in_addr server_addr;
	memcpy(&server_addr, host->h_addr, sizeof(server_addr));
	sprintf(ip_address, "%s", inet_ntoa(server_addr));
}

void  get_server_info(struct sockaddr_in server_info, char *hostname)
{
	char temp[MAX_HOST_NAME];
	struct utsname sysname;
	memset(&sysname, 0, sizeof(sysname));
	if (-1 == uname(&sysname)) {
		ERROR_EXIT("Failed to run uname [%s]\n", strerror(errno));
	}
	memcpy(temp, sysname.nodename, sizeof(sysname.nodename));
	memcpy(hostname, temp, strlen(temp)+1);

	struct hostent *host  = gethostbyname(temp);
	if (NULL == host) {
		ERROR_EXIT("Failed to run gethostbyname [%s]\n", strerror(errno));
	}

	memset(&server_info, 0, sizeof(server_info));
	memcpy(&server_info.sin_addr, host->h_addr, host->h_length);
}

void bind_address_to_server_socket(int listen_socket, char *hostname)
{
	// Turn off bind address checking & allow port numbers to be re-used
	int on = 1;
	set_socket_options(listen_socket, SO_REUSEADDR, (const char *)&on, sizeof(on));

	// When connection closed we need to ensure all data is transmitted
	struct linger linger;
	linger.l_onoff  = 1;
	linger.l_linger = 30;

	set_socket_options(listen_socket, SO_LINGER, (const char *)&linger, sizeof(linger));

	struct sockaddr_in server_info;
	server_info.sin_family          = AF_INET;

	// Allow server to be contacted on any Ip address
	server_info.sin_addr.s_addr     = htonl(INADDR_ANY);
	server_info.sin_port            = htons(PORT_NUMBER);
	get_server_info(server_info, hostname);
	PRINTF("SERVER INFO: %s : %d\n", hostname, PORT_NUMBER);

	if ( -1 == bind(listen_socket, (struct sockaddr *) &server_info, sizeof(server_info))) {
		ERROR_EXIT("Failed to bind socket [%s]\n", strerror(errno));
	}
}

void listen_on_server_socket(int listen_socket, char *port_num)
{
	if (-1 == listen(listen_socket, MAX_CONNECTION)) {
		ERROR_EXIT("Failed to listen socket [%s]\n", strerror(errno));
	}
	struct sockaddr_in temp_sa;
	socklen_t temp_sa_len = sizeof(temp_sa);
	if (-1 == getsockname(listen_socket, (struct sockaddr *) &temp_sa, &temp_sa_len))  {
		ERROR_EXIT("Failed to run getsockname [%s]\n", strerror(errno));
	}
	sprintf(port_num, "%hu", htons(temp_sa.sin_port));
}


int create_server()
{
	char hostname[MAX_HOST_NAME];
	char portnum[8];

	int listen_socket = create_server_socket();
	bind_address_to_server_socket(listen_socket, hostname);
	listen_on_server_socket(listen_socket, portnum);
	return listen_socket;
}
