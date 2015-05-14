#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sched.h>
#include <pthread.h>

#define MAX_QUEUE_LENGTH 5
#define MAX_THREADS 5
#define MAX_JOBS 5
pthread_t *g_prod_thread_id = NULL;
pthread_t *g_cons_thread_id = NULL;
static pthread_mutex_t g_print_mutex    = PTHREAD_MUTEX_INITIALIZER;

struct job {
	int id;
	char *name;	
};
typedef struct job job_t;

struct circular_queue {
	int 		    max_queue_len;
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


void my_printf(const char *func, const unsigned int line, char *fmt, ...)
{
	pthread_mutex_lock(&g_print_mutex);
	va_list ap;
	fprintf(stderr, "%s(%d) : ", func, line); 	
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);	
	va_end(ap);
	pthread_mutex_unlock(&g_print_mutex);	
}

#define PRINTF(format, ...) my_printf(__FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define WAIT_IN_SECONDS 1

void thread_cond_timed_wait(pthread_mutex_t *mutex, pthread_cond_t *cond)
{
	struct timespec ts;
    	struct timeval tp;
    	int ret;    	
    	pthread_mutex_lock(mutex);
    	gettimeofday(&tp, NULL);    		
    	ts.tv_sec  = tp.tv_sec;
    	ts.tv_nsec = tp.tv_usec * 1000;
    	ts.tv_sec += WAIT_IN_SECONDS;    	
 	ret = pthread_cond_timedwait(cond, mutex, &ts);
 	if (ret == ETIMEDOUT) { 		
 		sched_yield(); 		
 	}
 	pthread_mutex_unlock(mutex);
}

/*
 * spin_lock is used to protect queue & its pointers 
 * mutex_full & cond_full are used to protect insert threads
 * mutex_empty & cond_empty are used to protect delete threads
 *
 */

 /* Blocking insert :  waits until jobs are deleted from queue */ 
 void insert_to_queue(job_t *job, circular_queue_t *ptr)
 {
 	pthread_spin_lock(&ptr->spin_lock); 	
 	while (((ptr->rear + 1) % MAX_QUEUE_LENGTH) == ptr->front) {
 		pthread_spin_unlock(&ptr->spin_lock); 		 		
    		//PRINTF("Job - %d: Circular Queue is full thread waiting here [front=%d rear=%d]\n", job->id, ptr->front, ptr->rear);    		
    		thread_cond_timed_wait(&ptr->mutex_full, &ptr->cond_full);
 		pthread_spin_lock(&ptr->spin_lock); 		
 	}
 	if(ptr->front == -1)
 		ptr->front = ptr->rear = 0;
 	else 
 		ptr->rear = (ptr->rear + 1) % MAX_QUEUE_LENGTH; 
 	ptr->queue[ptr->rear] = job;
 	pthread_spin_unlock(&ptr->spin_lock);


  	/* Notify of not empty */
 	pthread_mutex_lock(&ptr->mutex_empty);
  	//if(job) PRINTF("job-%d: Circular Queue notifying not empty [front=%d rear=%d]\n", job->id, ptr->front, ptr->rear); 	
 	pthread_cond_broadcast(&ptr->cond_empty);
 	pthread_mutex_unlock(&ptr->mutex_empty); 	
 }

 /* Blocking delete :  waits until job available */ 
 job_t *delete_from_queue(circular_queue_t *ptr)
 {
 	job_t *job = NULL; 	
 	pthread_spin_lock(&ptr->spin_lock); 	
 	while (ptr->front == -1) {
 		pthread_spin_unlock(&ptr->spin_lock);
      		//PRINTF("Circular Queue is empty waiting here [front=%d rear=%d]\n", ptr->front, ptr->rear);
      		thread_cond_timed_wait(&ptr->mutex_empty, &ptr->cond_empty);
 		pthread_spin_lock(&ptr->spin_lock);
 	}
 	job    = ptr->queue[ptr->front];
 	if (ptr->front == ptr->rear)
 		ptr->front=ptr->rear = -1;
 	else
 		ptr->front = (ptr->front + 1) % MAX_QUEUE_LENGTH; 	
 	pthread_spin_unlock(&ptr->spin_lock);
 	 	
  	/* Notify of not full */  	
 	pthread_mutex_lock(&ptr->mutex_full); 	
  	//if(job) PRINTF("Job-%d: Circular Queue notifying not full [front=%d rear=%d] \n", job->id ,ptr->front, ptr->rear); 	
 	pthread_cond_broadcast(&ptr->cond_full);
 	pthread_mutex_unlock(&ptr->mutex_full); 	
 	return job;
 }


 void cleanup_circular_queue(circular_queue_t *ptr)
 {
    	// Destory Queue data structures
 	pthread_cond_destroy(&ptr->cond_empty); 
 	pthread_cond_destroy(&ptr->cond_full);
 	pthread_mutex_destroy(&ptr->mutex_empty); 
 	pthread_mutex_destroy(&ptr->mutex_full); 
 	pthread_spin_destroy(&ptr->spin_lock); 		
 }

 void init_circular_queue(circular_queue_t *ptr, int num_jobs)
 {
     	// Init Queue data structure
 	ptr->max_queue_len= num_jobs; 	
 	ptr->front        = -1;
 	ptr->rear         = -1; 	
 	pthread_cond_init (&ptr->cond_empty, NULL); 
 	pthread_cond_init (&ptr->cond_full, NULL);
 	pthread_mutex_init(&ptr->mutex_empty, NULL); 
 	pthread_mutex_init(&ptr->mutex_full, NULL); 
 	pthread_spin_init (&ptr->spin_lock, 0);
 }


void *fire_jobs(void *arg)
{
	circular_queue_t *cq = (circular_queue_t *) arg;
	unsigned int i;
	for(i = 0; i < MAX_JOBS; i++) { // every thread creates MAX jobs
		job_t *job = (job_t *) calloc(1, sizeof(job_t));
		job->id = i;
		char tmp[20];
		sprintf(tmp, "Thread_%x_Id_%d", (unsigned int)pthread_self(), i);
		job->name = strdup(tmp);
		PRINTF("Submitting job %d : %s \n", job->id, job->name);		
		insert_to_queue(job, cq);		
	}
	return NULL;
}

void create_producers(circular_queue_t *cq)
{
	unsigned int i;
	g_prod_thread_id = (pthread_t *) calloc(MAX_THREADS, sizeof(pthread_t));
    	for(i = 0; i < MAX_THREADS; i++) 
        	pthread_create(&g_prod_thread_id[i], NULL, fire_jobs, cq);
}

void *run_jobs(void *arg) 
{
	circular_queue_t *cq = (circular_queue_t *) arg;
	while(1) {
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		job_t *job = delete_from_queue(cq);
		if(job) {
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			PRINTF("Running job %d : %s \n", job->id, job->name);
			free(job->name);
			free(job);
		} else break; // if job is NULL kill thread
	}
	return NULL;
}

void create_consumers(circular_queue_t *cq)
{
	unsigned int i;
	g_cons_thread_id = (pthread_t *) calloc(MAX_THREADS, sizeof(pthread_t));
    	for(i = 0; i < MAX_THREADS; i++) 
        	pthread_create(&g_cons_thread_id[i], NULL, run_jobs, cq);
}

void cleanup_threads()
{       
    free(g_prod_thread_id);
    free(g_cons_thread_id);    
}

int main(int argc, char *argv[]) {
	unsigned int i;
	void *exit_status;
	circular_queue_t *cq = (circular_queue_t *) calloc(1, sizeof(circular_queue_t));
	init_circular_queue(cq, MAX_JOBS);

	create_producers(cq);
	create_consumers(cq);        

	// Wait until all producer threads exit
    	for(i = 0; i < MAX_THREADS; i++) 
        	pthread_join(g_prod_thread_id[i], &exit_status);

        for(i = 0; i < MAX_THREADS; i++) // For consumer threads to exit
        	insert_to_queue(NULL, cq);

	// Wait until all consumer threads exit
    	for(i = 0; i < MAX_THREADS; i++) 
        	pthread_join(g_cons_thread_id[i], &exit_status);

	cleanup_threads();
        cleanup_circular_queue(cq); 
        free(cq); 
        return 0;   
}
