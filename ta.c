#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3


/*
This program is the solution for the Teaching assistant problem where there is a TA who helps students but only one at time.
There are 3 chairs in which the students can wait and if more students come then can revisit after sometime for the help.
If no students are there in the waiting chairs then the TA goes into sleeping mode.

For synchronization semaphores have been used and each individual student and TA are created by seperate threads.
*/
sem_t mutex,students,ta; //initilazing 3 semaphores
int numberOfSeatsWR = N; //The number of chairs empty that are empty

void ta_p(void *arg); //function for teaching assistent
void student_p(void *arg); //function for student

int main(int argc,char *argv[])
{
	int std,status;
	printf("Enter the number of students:");
	scanf("%d",&std);
	printf("\n");
    	pthread_t TA,student[std];


    	//Semaphore initialization
    	sem_init(&mutex,0,1);
    	sem_init(&students,0,0);
    	sem_init(&ta,0,1);


    	//Student thread creation
	for(int i=0;i<std;i++)
    	{
		status=pthread_create(&student[i],NULL,(void *)student_p,(void*)(long)i);
		if (status!=0)
			perror("Thread creation failed\n");
	}
	//Teaching assistant thread creation
	status = pthread_create(&TA,NULL,(void *)ta_p,NULL);
	if(status!=0)
		perror("Thread creation failed");

	for(int i=0;i<std;i++)
	    pthread_join(student[i],NULL);

	pthread_join(TA,NULL);

	sem_destroy(&mutex);
	sem_destroy(&students);
	sem_destroy(&ta);

	return 0;
}

//teaching assistent process
void ta_p(void *arg)
{
	while(1)
    	{
		sem_wait(&students);
        	sem_wait(&mutex);
		if(numberOfSeatsWR == 2)
			printf("TA has be woken up\n");
        	numberOfSeatsWR++;
        	printf("TA tutoring, No of empty seats :%d.\n",numberOfSeatsWR);
        	sem_post(&mutex);
        	sem_post(&ta);
        	sleep(3);
        	printf("TA tutoring completed.\n");
        	if(numberOfSeatsWR == N)
            		printf("TA goes to sleep.\n");
    	}
}

//student process
void student_p(void *arg)
{
    	while(1)
    	{
		int t = rand()%10 + 1;
		printf("Student %ld is studying on own for %d seconds\n",(long)arg+1,t);
		sleep(t);
		sem_wait(&mutex);
        	if(numberOfSeatsWR>0)
        	{
            		numberOfSeatsWR--;
            		printf("Student %ld arrived in waiting room, No of empty seats :%d\n",(long)arg+1,numberOfSeatsWR);
            		sem_post(&students);
			sem_post(&mutex);
			sem_wait(&ta);
        	}
        	else
		{
            		sem_post(&mutex);
            		printf("Student %ld sees that there is no empty chair and leaves.\n",(long)arg+1);
	    		//sleep(3);
        	}
    	}
}
