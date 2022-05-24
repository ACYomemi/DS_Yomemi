#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
sem_t full;
sem_t empty;
pthread_mutex_t lock;
int buff[10];
int buff_count;
void *Producer(void* arg)
{
	pthread_t id=pthread_self();
	printf("[Producer] Process %lu is running\n",(unsigned long)id);
	int i=0;
	for (i=0;i<10;i++)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&lock);
		buff[buff_count]=1;
		buff_count++;
		printf("[Producer] buffer count=%d\n",buff_count);
		pthread_mutex_unlock(&lock);
		sem_post(&full);
		sleep(2);
	}
	printf("[Producer] Process %lu exited normally\n",(unsigned long)id);
	return 0;
}
void *Consumer(void* arg)
{
	pthread_t id=pthread_self();
	printf("[Consumer] Process %lu is running\n",(unsigned long)id);
	int i=0;
	for (i=0;i<10;i++)
	{
		sem_wait(&full);
		pthread_mutex_lock(&lock);
		buff[buff_count]=0;
		buff_count--;
		printf("[Consumer] buffer count=%d\n",buff_count);
		pthread_mutex_unlock(&lock);
		sem_post(&empty);
		sleep(3);
	}
	printf("[Consumer] Process %lu exited normally\n",(unsigned long)id);
	return 0;
}
int main(void)
{
	pthread_t id=pthread_self();
	printf("[main] Process %lu is running\n",(unsigned long)id);
	int i=0;
	for (i=0;i<20;i++)
		buff[i]=0;
	buff_count=0;
	sem_init(&full,0,0);
	sem_init(&empty,0,10);
	pthread_mutex_init(&lock,NULL);
	pthread_t Pro,Con;
	pthread_create(&Pro,NULL,Producer,NULL);
	pthread_create(&Con,NULL,Consumer,NULL);
	pthread_join(Pro,NULL);
	pthread_join(Con,NULL);
	sem_destroy(&full);
	sem_destroy(&empty);
	pthread_mutex_destroy(&lock);
	printf("[main] Process %lu exited normally\n",(unsigned long)id);
	return 0;
}
