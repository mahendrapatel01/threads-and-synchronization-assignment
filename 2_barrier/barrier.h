#include <pthread.h>

struct barrier_t
{
	/*
		Todo
		Barrier related variables
	*/
	int n;
	int count ;
	pthread_mutex_t cnt_mutex;
	pthread_cond_t cv1;
	pthread_cond_t cv2;
	
};

void barrier_init(struct barrier_t *b, int i);
void barrier_wait(struct barrier_t *b);
