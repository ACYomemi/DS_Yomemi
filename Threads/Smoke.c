#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
sem_t empty;
sem_t supply[3];
pthread_mutex_t desk;
int cur;
int par[3]={0,1,2};
void *Marchant(void* arg)
{
	pthread_t kid=pthread_self();
        printf("[Marchant] Process %lu is running\n",(unsigned long)kid);
	int i=0;
	for (i=0;i<30;i++)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&desk);
		cur=i%3;
		printf("[Marchant] Produced a supply(%d)\n",cur);
		pthread_mutex_unlock(&desk);
		sem_post(&supply[i%3]);
		sleep(2);
	}
	printf("[Marchant] Process %lu exited normally\n",(unsigned long)kid);
	return 0;
}
void *Smoker(void* arg)
{
	int id=*(int*)arg;
	pthread_t kid=pthread_self();
        printf("[Smoker %d] Process %lu is running\n",id,(unsigned long)kid);
	int i=0;
	for (i=0;i<10;i++)
	{
		sem_wait(&supply[id]);
		pthread_mutex_lock(&desk);
		printf("[Smoker %d] Taking supply(%d) and smoking\n",id,cur);
		cur=-1;
		pthread_mutex_unlock(&desk);
		sem_post(&empty);
		sleep(8);
	}
	printf("[Smoker %d] Process %lu exited normally\n",id,(unsigned long)kid);
        return 0;
}
int main(void)
{
	pthread_t kid=pthread_self();
        printf("[main] Process %lu is running\n",(unsigned long)kid);
	printf("For these supplies,\n0=tabacco and paper, 1=tabacco and match, 2=paper and match.\n");
	int i=0;
	cur=-1;
	sem_init(&empty,0,1);
	pthread_mutex_init(&desk,NULL);
	for (i=0;i<3;i++)
		sem_init(&supply[i],0,0);
	pthread_t id[4];
        for (i=0;i<3;i++)
        {
                int* p=&par[i];
                pthread_create(&id[i],NULL,Smoker,p);
        }
	pthread_create(&id[3],NULL,Marchant,NULL);
	for (i=0;i<3;i++)
                pthread_join(id[i],NULL);
	sem_destroy(&empty);
        pthread_mutex_destroy(&desk);
	for (i=0;i<3;i++)
		sem_destroy(&supply[i]);
	printf("[main] Process %lu exited normally\n",(unsigned long)kid);
	return 0;

}
