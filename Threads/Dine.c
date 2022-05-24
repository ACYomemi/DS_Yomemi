#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
sem_t chopstick[5];
pthread_mutex_t isusing[5];
int cur[5];
int par[5]={0,1,2,3,4};
void *Thinker(void* arg)
{
	int id=*(int*)arg;
	pthread_t kid=pthread_self();
	printf("[Thinker %d] Process %lu is running\n",id,(unsigned long)kid);
	int sl=(id+2)%3+1;
	int i=0;
	for (i=0;i<10;i++)
	{
		sem_wait(&chopstick[id]);
		pthread_mutex_lock(&isusing[id]);
		sem_wait(&chopstick[(id+1)%5]);
		pthread_mutex_lock(&isusing[(id+1)%5]);
		cur[id]=1;
		cur[(id+1)%5]=1;
		printf("[Thinker %d] Eating by chopstick %d and %d\n",id,id,(id+1)%5);
		cur[id]=0;
		cur[(id+1)%5]=0;
		pthread_mutex_unlock(&isusing[(id+1)%5]);
		sem_post(&chopstick[(id+1)%5]);
		pthread_mutex_unlock(&isusing[id]);
		sem_post(&chopstick[id]);
		sleep(3);
	}
	printf("[Thinker %d] Process %lu exited normally\n",id,(unsigned long)kid);
	return 0;
}
int main(void)
{
	pthread_t kid=pthread_self();
        printf("[main] Process %lu is running\n",(unsigned long)kid);
	int i=0;
	for (i=0;i<5;i++)
	{
		cur[i]=0;
		sem_init(&chopstick[i],0,1);
		pthread_mutex_init(&isusing[i],NULL);
	}
	pthread_t id[5];
	for (i=0;i<5;i++)
	{
		pthread_create(&id[i],NULL,Thinker,&par[i]);
	}
	for (i=0;i<5;i++)
		pthread_join(id[i],NULL);
	for (i=0;i<5;i++)
	{
		sem_destroy(&chopstick[i]);
		pthread_mutex_destroy(&isusing[i]);
	}
	printf("[main] Process %lu exited normally\n",(unsigned long)kid);
	return 0;
}

