#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef __linux__
#include <sys/sendfile.h>
#else
#include <sys/uio.h>
#endif
#include <netinet/in.h>
#include <sys/utsname.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/param.h>
#include "defines.hpp"
#include <string>

extern FILE *g_stream;

void log_message(char type, FILE *stream, char const *func,
				 const unsigned int line, char const *fmt, ...);

void create_log_file();
void recreate_log_file();

//#ifdef DEBUG
#define TRACE(format, ...) 	log_message('t', g_stream, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define TRACE_MSG(stmt)		stmt
//#else
//#define TRACE(format, ...) 	(void)0
//#define TRACE_MSG(stmt)		(void)0
//#endif

#define ERROR_EXIT(format, ...) { \
					log_message('e', g_stream, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
					log_message('e', stderr, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
					exit(1); \
				}

#define PRINTF(format, ...)     { \
					log_message('p', g_stream, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
					log_message('p', stdout, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); }

#define ERROR(format, ...) { \
					log_message('e', g_stream, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
					log_message('e', stderr, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); }

struct job {
	int sd;
	char *buff;
};
typedef struct job job_t;

struct circular_queue {
	int 		    	max_queue_len;
	job_t               *queue[MAX_QUEUE_LENGTH];
	int                 front;
	int                 rear;
	pthread_mutex_t     mutex_full;
	pthread_mutex_t     mutex_empty;
	pthread_cond_t      cond_full;
	pthread_cond_t      cond_empty;
	pthread_spinlock_t  spin_lock;
};
typedef struct circular_queue circular_queue_t;

int create_server();
int query_execute(int argc, char *argv[], int sd);

void init_circular_queue(circular_queue_t *ptr, int num_jobs);
void insert_to_queue(job_t *job, circular_queue_t *ptr);
job_t *delete_from_queue(circular_queue_t *ptr);
void cleanup_circular_queue(circular_queue_t *ptr);

void create_threads(int listen_socket, circular_queue_t *queue);
void thread_clean_up();

int create_temp_file(char *tmp_name);
void remove_temp_file(int fd, char *tmp_name);

#endif
