#include<stdio.h>
#include<stdlib.h>
#include"pthread.h"
#include<unistd.h>

/* 
 * This is the counter value which is to
 * be incremented by all the threads created 
 * by your program
*/
pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void *pthread_counter_fun(void* args[])
{
	pthread_mutex_lock(&c_mutex);
	counter++;
	pthread_mutex_unlock(&c_mutex);
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	/* Write you code to create n threads here*/
	/* Each thread must increment the counter once and exit*/
	int  n = atoi(argv[1]);
	//printf("%d\n",n);
	int i;
	pthread_t *threads_id = (pthread_t*)malloc(sizeof(pthread_t)*n);
	for(i=0;i<n;i++)
	{
		//pritnf()
		pthread_create(&threads_id[i],NULL,(void*)pthread_counter_fun,NULL);
	} 
	for(i=0;i<n;i++)
	{
		pthread_join(threads_id[i],NULL);
	}

	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	printf("%d\n", counter);
	sleep(10000);
	return 0;
}
