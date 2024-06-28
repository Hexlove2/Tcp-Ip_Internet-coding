#include <stdio.h>
#include <pthread.h>
void *thread_summation(void * arg);
int sum=0;

int main(int argc, char const *argv[])
{
    pthread_t id_t1, id_t2;
    int range1[]={1,100};
    int range2[]={100,1000};

    pthread_create(&id_t1,NULL,thread_summation,(void*)range1);
    pthread_create(&id_t2,NULL,thread_summation,(void*)range2);

    pthread_join(id_t1,NULL);
    pthread_join(id_t2,NULL);
    printf("Sum: %d\n",sum);
    return 0;
}

void *thread_summation(void * arg)
{
    int start=((int*)arg)[0];
    int end=((int*)arg)[1];
    while(start<=end)
    {
        printf("Now: + %d \n",start);
        sum+=start;
        start++;
    }
    return NULL;
}