#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define NT 1000
/* 
 * This is the data array which is to
 * be incremented by all the threads created 
 * by your program as specified in the problem state
*/
int data[10];

pthread_mutex_t cm_array[10];

int index_arr[10] = {0,1,2,3,4,5,6,7,8,9};
void *count_data(void *args)
{
	int *p = (int*)args;
	for(int i=0;i<NT;i++)
	{
		pthread_mutex_lock(&cm_array[*p]);
	//	printf("thread incrementing %d : %d\n",*p,data[*p]);
		data[*p]++;
		pthread_mutex_unlock(&cm_array[*p]);
	}
	//printf("exiting thread : %d\n",*p);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	/* Write you code to create 10 threads here*/
	/* Increment the data array as specified in the problem statement*/
	pthread_t thread_id[10];
	for(int i=0;i<10;i++){
		pthread_mutex_init(&cm_array[i],NULL);
		data[i]=0;
	}
	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	for(int i=0;i<10;i++)
	{	
	//	printf("creating %d thread\n",i);
		pthread_create(&thread_id[i],NULL,(void*)&count_data,(void*)&index_arr[i]);	
	}	
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<NT;j++)
		{
			pthread_mutex_lock(&cm_array[i]);
	//		printf("parent incrementing %d : %d\n",i,data[i]);
			data[i]++;
			pthread_mutex_unlock(&cm_array[i]);
		}
	}
	
	for(int i=0;i<10;i++){
	//	printf("join thread : %d\n",i);
		pthread_join(thread_id[i],NULL);
	}
		
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n", data[i]);
	}
	
	sleep(1);
	return 0;
}
