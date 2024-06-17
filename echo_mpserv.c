#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char const *argv[])
{
    //服务器socket相关
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;

    char buf[BUF_SIZE];
    //多进程相关
    pid_t pid;
    struct sigaction act;
    int str_len, state;
    
    //初始化完成
    if(argc!=2)
     {
        printf("Usage:%s <port> \n", argv[0]);
        exit(1);
     }

    //多进程初始化
    act.sa_handler=read_childproc;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);

    //绑定信号,当子进程结束时调用
    state=sigaction(SIGCHLD, &act, 0);

    //服务器套接字初始化
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    //绑定
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
     error_handling("bind error");
    
    //监听
    if(listen(serv_sock, 5)==-1)
     error_handling("listen error");

    //多进程服务
    while(1)
    {
        //客户端套接字创建
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

        //多进程时，要满足多个客户端
        if(clnt_sock==-1)
         continue;
        else
         puts("new client connected");

        //开启多进程
        //父进程pid为本身，而子进程的pid则为0
        pid=fork();

        //假设进程创建失败
        if(pid==-1)
        {
            close(clnt_sock);
            continue;
        }
        //子进程运行区域
        if(pid==0)
        {
            //此时可关闭服务器初始套接字,不会影响父进程中的套接字
            close(serv_sock);

            //服务客户端
            //echo,不过不是按字节读取，当读取内容过大时可能造成问题
            while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
                write(clnt_sock, buf, str_len);
            
            //关闭套接字
            close(clnt_sock);
            puts("client disconnected");

            //关闭子进程，且会调用子进程结束函数,不会出现僵尸进程
            return 0;
        }

        //父进程运行区域
        else
        {
            //因为此时子进程已经复制了,可以直接关掉,只有当所有进程中的文件描述符均关闭后，套接字才会消失
            close(clnt_sock);
        }
    }

    //关闭服务器套接字
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
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
