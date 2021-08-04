#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barrier.h"

void barrier_init(struct barrier_t * barrier, int nthreads)
{
	/*
		Todo
		intailize barrier
	*/
	printf("barrier init start\n");
	barrier->n = nthreads;
	barrier->count = 0;
	pthread_mutex_init(&barrier->cnt_mutex,NULL);
	pthread_cond_init(&barrier->cv1 ,NULL);
	pthread_cond_init(&barrier->cv2 ,NULL);
	//pthread_cond_wait(&barrier->cv1,&barrier->cnt_mutex);
	//pthread_cond_wait(&barrier->cv2,&barrier->cnt_mutex);
	printf("barrier init over\n");
	return;
}
/*
void barrier_wait(struct barrier_t *barrier)
{
	
		Todo
	
	printf("barrrier wait start\n");
	pthread_mutex_lock(&(barrier->cnt_mutex));
		printf("bw ts1 cnt++ %d \n",barrier->count);
		barrier->count++;
		if(barrier->count==barrier->n)
		{
		//pthread_cond_wait(&(barrier->cv2),&(barrier->cnt_mutex));
			pthread_cond_broadcast(&(barrier->cv1));
		}
		else
		{
			pthread_cond_wait(&(barrier->cv1),&(barrier->cnt_mutex));
			pthread_mutex_unlock(&(barrier->cnt_mutex));
		}
	pthread_cond_signal(&(barrier->cv1));
	
	pthread_mutex_lock(&(barrier->cnt_mutex));
	barrier->count--;
	if(barrier->count==0)
	{
	//	pthread_cond_wait(&(barrier->cv1),&(barrier->cnt_mutex));
		pthread_cond_broadcast(&(barrier->cv2));
	}
	else
	{
		pthread_cond_wait(&(barrier->cv2),&(barrier->cnt_mutex));
		pthread_mutex_unlock(&(barrier->cnt_mutex));
	}
	
	pthread_cond_signal(&(barrier->cv2));
	printf("barrrier wait over\n");
	return;
}
*/
void barrier_wait(struct barrier_t *barrier)
{
	/*
		Todo
	*/
	pthread_mutex_lock(&(barrier->cnt_mutex));
	barrier->count++;
	//printf("barrier cnt %d : \n",barrier->count);
	if(barrier->count==barrier->n)
	{
		barrier->count = 0;
		pthread_cond_broadcast(&(barrier->cv1));
	}
	else{
		pthread_cond_wait(&(barrier->cv1),&(barrier->cnt_mutex));
	}
	pthread_mutex_unlock(&(barrier->cnt_mutex));
	return;
}
