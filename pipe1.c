#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30
int main(int argc, char const *argv[])
{
    pid_t pid;
    //create two pipe
    int fds1[2],fds2[2];
    char mes1[]="Who are you?";
    char mes2[]="I am your son.";
    char buf[BUF_SIZE];

    pipe(fds1);
    pipe(fds2);

    pid=fork();
    if(pid==0)
    {
        write(fds1[1], mes1, sizeof(mes1));
        read(fds2[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n", buf);
    }
    else
    {
        read(fds1[0], buf, BUF_SIZE);
        printf("Parent proc output: %s \n", buf);
        write(fds2[1], mes2, sizeof(mes2));
        sleep(3);
    }

    return 0;
}
