#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char const *argv[])
{
    pthread_t id_t1,id_t2;
    sem_init(&sem_one, 0 ,0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&id_t1,NULL,read,NULL);
    pthread_create(&id_t2,NULL,accu,NULL);

    pthread_join(id_t1,NULL);
    pthread_join(id_t2,NULL);   

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

//确保读入了数据才能累加，确保累加完毕才能继续读入数据，因此两个同步信号量
void * read(void * arg)
{
    int i;
    for(i=0;i<5;i++)
    {
      fputs("Input:",stdout);
      sem_wait(&sem_two);
      scanf("%d",&num);
      sem_post(&sem_one);
    }
    return NULL;
}

void * accu(void * arg)
{
    int i,sum;
    sum=0;
    for(i=0;i<5;i++)
    {
        sem_wait(&sem_one);
        sum+=num;
        sem_post(&sem_two);
    }
    printf("result: %d \n",sum);
    return NULL;
}