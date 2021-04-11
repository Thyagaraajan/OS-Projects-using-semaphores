#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


// The solution for this problem is the  act of grabing the left and the right fork is atomic.


enum State 
{
	EATING,
	THINKING,
	HUNGRY
};

int num = 0;
int *states;
sem_t *forks;
sem_t mutex;

char getStateName(int state)
{
	switch(state)
	{
		case EATING: return 'E'; break;
		case THINKING: return 'T'; break;
		case HUNGRY: return 'H'; break;
	}
}

void* philosopher (void* arg)
{
	
	int n =*((int *)arg);
	int left = n;
	int right = (n + 1) % num;
	while(1)
	{ 		
			
		/* GET THE FORKS */ 
		
		sleep(rand()%10+1);
		
		/* The philosopher is going to take the fork so we are decrementing the semaphore variable*/	
		
		states[n] = HUNGRY;
		sem_wait(&mutex);		

		sem_wait(&forks[left]);
		sem_wait(&forks[right]);
		states[n] = EATING;

		sem_post(&mutex);

		sleep(rand()%10+1);

		/* The philosopher ate, so he is dropping the fork, so we are incrementing the semaphore variable. */
		sem_post(&forks[left]);
		sem_post(&forks[right]);
		states[n] = THINKING;
		
	}	
}


int main(int argc, char *argv[ ]) 
{
    
	if (argc > 1)
	{
		num = atoi(argv[1]);
	}
	else
	{
		printf("Define the number of philosophers!");
		return -1;
	}

	int i;
	states = malloc(sizeof(int)*num);	
	
	for (i=0; i<num; i++)
	{
		states[i] = THINKING;
	}
	
	forks = malloc(sizeof(sem_t)*num);
	
	sem_init(&mutex, 0, 1);
	pthread_t philosophers[num];
	
	for(i=0; i<num;i++)
	{  		
		//initialize semaphores for forks all in 1
    		sem_init(&forks[i], 0, 1);
			int *arg= malloc (sizeof(int));
			*arg = i;		
    		//create thread for each philosopher
    		pthread_create(&philosophers[i], NULL, philosopher, (void *)arg);
	}
	// print of states
	char state;
	int j;
	while(1)
	{
		sleep(1);

    		for (j=0; j<num; j++)
		{
			state = getStateName(states[j]);
			if(j!=num-1)
			{
			    printf(" %c - ", state);
			}
			else
			{
			    printf(" %c ",state);
			}
		}
		printf("\n");
		
	}
    	for (i = 0; i < num; i++)
    	{
        	pthread_join(philosophers[i], NULL);
    	}

	pthread_exit(0); 
}