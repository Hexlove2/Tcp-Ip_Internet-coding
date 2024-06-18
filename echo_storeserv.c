#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char const *argv[])
{
    //about sock
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    //pid
    pid_t pid;

    //pipe
    int fds[2];

    //signal
    struct sigaction act;

    //other
    char buf[BUF_SIZE];
    int state, str_len;

    if(argc!=2)
    {
        printf("Usage: %s <port>",argv[0]);
        exit(1);
    }

    //signal set
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state=sigaction(SIGCHLD, &act, 0);

    //socket set
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
     error_handling("bind error");
    
    if(listen(serv_sock,5)==-1)
     error_handling("listen error");
    
    //using pipe to communicate with father process
    pipe(fds);
    //create a new process to write message to file
    pid=fork();
    //son process
    if(pid==0)
    {
        FILE * fp = fopen("echomsg.txt","wt");
        char msgbuf[BUF_SIZE];
        int i,len;
        for(i=0;i<10;i++)
        {
            len=read(fds[0],msgbuf,BUF_SIZE);
            //printf("form write pipe:%s",msgbuf);
            fwrite((void *)msgbuf,1,len,fp);
        }
        fclose(fp);
        return 0;
    }

    //echo multiprocessing
    while(1)
    {
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
        if(clnt_sock==-1)
            continue;
        else
            puts("new client connected");
        
        pid=fork();
        if(pid==0)
        {
            close(serv_sock);
            while((str_len=read(clnt_sock,&buf,BUF_SIZE))!=0)
            {
                //to client
                write(clnt_sock,buf,str_len);
                //to pipe
                write(fds[1],buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else
        {
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    //不会造成阻塞，且此时子进程一定已结束
    pid=waitpid(-1, &status, WNOHANG);
    printf("removed proc id :%d \n",pid);
}