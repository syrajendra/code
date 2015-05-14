#include <common.hpp>
 #include <sys/time.h>

#define WAIT_IN_SECONDS 1/2


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
