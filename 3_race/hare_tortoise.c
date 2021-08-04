#include "cs_thread.h"
#include<pthread.h>
#define true 1
#define false 0
struct Repositioning {
	char player; 		// T for turtle and H for hare
	int time; 		// At what time god interrupt's
	int distance;		// How much does god move any of the player. 
							// distance can be negetive or positive.
							// using this distance if any of players position is less than zero then bring him to start line.
							// If more than finish_distance then make him win.
							// If time is after completion of game than you can ignore it as we will already have winner.
};

struct race {
	
	//	Don't change these variables.
	//	speeds are unit distance per unit time.
	int printing_delay;
	int tortoise_speed;					// speed of Turtle
	int hare_speed;						// speed of hare
	int hare_sleep_time; 				// how much time does hare sleep (in case he decides to sleep)
	int hare_turtle_distance_for_sleep; // minimum lead which hare has on turtle after which hare decides to move
										// Any lead greater than this distance and hare will ignore turtle and go to sleep
	int finish_distance;				// Distance between start and finish line
	struct Repositioning* reposition;	// pointer to array containing Randomizer's decision
	int repositioning_count;			// number of elements in array of repositioning structure
	
	//	Add your custom variables here.
	int hare_dist;
	int turtle_dist;
	int time_count;
	int hare_sleep_count;
	pthread_mutex_t lck0;		
	pthread_mutex_t lck1;	
	pthread_mutex_t lck2;	
	pthread_mutex_t lck3;	
	pthread_mutex_t lck4;	
};


void* Turtle(void *race);
void* Hare(void *race);
void* Randomizer(void *race);
void* Report(void *race);

char init(struct race *race)
{
	//printf("initailizing race customized variable \n");
	if(race->finish_distance==0 || race->hare_speed==0)
		return 'T';
	if(race->tortoise_speed==0)
		return 'H';
		
	race->time_count =0 ;
	race->hare_sleep_count=race->hare_sleep_time+1;
	race->hare_dist=0;
	race->turtle_dist=0;
	pthread_mutex_init(&race->lck0,NULL);
	pthread_mutex_init(&race->lck4,NULL);
	pthread_mutex_init(&race->lck1,NULL);
	pthread_mutex_init(&race->lck2,NULL);
	pthread_mutex_init(&race->lck3,NULL);

	//lock all the mutex of other thread 
	//printf("locking all lock var on other thread\n");
	pthread_mutex_lock(&race->lck0);
	pthread_mutex_lock(&race->lck1);
	pthread_mutex_lock(&race->lck2);
	pthread_mutex_lock(&race->lck3);
	
	pthread_t turtle_tid,hare_tid,rand_tid,report_tid;
	pthread_create(&turtle_tid,NULL,Turtle,(void*)race);
	pthread_create(&hare_tid,NULL,Hare,(void*)race);
	pthread_create(&rand_tid,NULL,Randomizer,(void*)race);
	pthread_create(&report_tid,NULL,Report,(void*)race);
	
	while(race->hare_dist < race->finish_distance && race->turtle_dist < race->finish_distance )
	{
		pthread_mutex_lock(&race->lck4);
		if(race->hare_dist >= race->finish_distance || race->turtle_dist >= race->finish_distance ){
	//	printf("inside init if !! \n");
		pthread_mutex_unlock(&race->lck3);
		break;	
		}	
	//	printf("\ninside init , time count : %d\n",race->time_count);
		race->time_count++;
//		printf("inside init after , time count : %d\n",race->time_count);
		pthread_mutex_unlock(&race->lck3);
	}
	pthread_join(rand_tid,NULL);
	pthread_join(turtle_tid,NULL);
	pthread_join(hare_tid,NULL);
	pthread_join(report_tid,NULL);
	
	if(race->turtle_dist >= race->hare_dist)
		return 'T';
	else 
		return 'H';
}

void* Turtle(void *arg)
{
	int check = true;
	struct race* race = (struct race*)arg;
	while(race->hare_dist < race->finish_distance && race->turtle_dist < race->finish_distance  )
	{
		check=true;
		pthread_mutex_lock(&race->lck2);
		if(race->hare_dist >= race->finish_distance || race->turtle_dist >= race->finish_distance ){
		
	//	printf("inside turtle if !! \n");
		pthread_mutex_unlock(&race->lck1);
		check=false;
		continue;			
		}
		race->turtle_dist += race->tortoise_speed;
//		printf("turtle distance : %d\n",race->turtle_dist);
		pthread_mutex_unlock(&race->lck1);
		check=false;
	}
	if(check)
		pthread_mutex_unlock(&race->lck1);
	return NULL;
  
}

void* Hare(void *arg)
{
	int check = true;
	struct race* race = (struct race*)arg;
	while(race->hare_dist < race->finish_distance && race->turtle_dist < race->finish_distance  )
	{
		pthread_mutex_lock(&race->lck1);
		check=true;
		if(race->hare_dist >= race->finish_distance || race->turtle_dist >= race->finish_distance ){
	//	printf("inside hare if !! \n");
		pthread_mutex_unlock(&race->lck0);
		check=false;
		continue;			
		}
		if(race->hare_sleep_count < race->hare_sleep_time )
		{
			race->hare_sleep_count++;
		}
		else
		{
		//	printf("inside hare else !! \n");
			if(race->hare_dist - race->turtle_dist <= race->hare_turtle_distance_for_sleep)
			{
			
			//	printf("inside hare else if !! \n");
				race->hare_dist += race->hare_speed;
			}
			race->hare_sleep_count=0;
		}
	//	printf("hare distance : %d\n",race->hare_dist);
		pthread_mutex_unlock(&race->lck0);
		check=false;
		
	}
	if(check)
		pthread_mutex_unlock(&race->lck0);
	return NULL;
}


void* Randomizer(void *arg)
{
	struct race* race = (struct race*)arg;
	int r=0;
	int check= true;
	while(race->hare_dist < race->finish_distance && race->turtle_dist < race->finish_distance )
	{
		pthread_mutex_lock(&race->lck3);
		check=true;
		if(race->hare_dist >= race->finish_distance || race->turtle_dist >= race->finish_distance  )
		{
		//	printf("inside Rand if !! \n");
			pthread_mutex_unlock(&race->lck2);
			check=false;
			continue;
		}
		if(r>=race->repositioning_count)
		{
		//	printf("inside rand if !! \n");
			pthread_mutex_unlock(&race->lck2);
			check= false;
			continue;
		}
		if(race->reposition[r].time == race->time_count)
		{
			if(race->reposition[r].player=='H')
			{
				race->hare_dist += race->reposition[r].distance; 
				if(race->hare_dist < 0)
					race->hare_dist = 0;
					
		//		printf("reposition hare distance : %d\n",race->hare_dist);
			}
			else
			{
				race->turtle_dist += race->reposition[r].distance;
				if(race->turtle_dist < 0)
					race->turtle_dist=0; 
					
		//		printf("repositiong turtle distance : %d\n",race->turtle_dist);
			}
			r++;
		}
		pthread_mutex_unlock(&race->lck2);
		check = false;
	}
	if(check)
		pthread_mutex_unlock(&race->lck2);
	return NULL;
}

void* Report(void *arg)
{
	struct race *race = (struct race*)arg;
	int check = true;
	while(race->hare_dist < race->finish_distance && race->turtle_dist < race->finish_distance  )
	{	
		pthread_mutex_lock(&race->lck0);
		check = true;
		if(race->printing_delay==0){
			pthread_mutex_unlock(&race->lck4);
			check=false;
			continue;	
		}
		if(race->time_count%race->printing_delay==0)
		{
			//printf("inside report !! \n");
			//printf("reporting status :\n ");
			printf("turtle distance : %d\n",race->turtle_dist);
			printf("hare distance : %d\n\n",race->hare_dist);
		}
		pthread_mutex_unlock(&race->lck4);
		check=false;	
	}
	if(check)
		pthread_mutex_unlock(&race->lck4);
	
	return NULL;
}

