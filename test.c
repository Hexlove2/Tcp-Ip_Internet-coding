#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

int main(int argc, char const *argv[])
{
    
    return 0;
}
void test1()
{
    int send_sock;
    int time_live=64;//TTL
    //UDP
    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,(void *)&time_live,sizeof(time_live));


    int recv_sock;
    struct ip_mreq join_adr;
    recv_sock=socket(PF_INET,SOCK_DGRAM,0);

    join_adr.imr_multiaddr.s_addr="多播组地址信息";
    join_adr.imr_interface.s_addr="加入多播组的主机地址信息";
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,(void *)&join_adr,sizeof(join_adr));
}
void test2()
{
    int send_sock;
    int bcast = 1;
    send_sock = socket(PF_INET , SOCK_DGRAM, 0);
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST,(void *)&bcast, sizeof(bcast));

    FILE * fdopen(int fildes, const char * mode);
    int fileno(FILE * stream);
}

void test3()
{
    /*
    int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t *attr);
    int pthread_mutex_destroy(pthread_mutex_t *mutex);

    int pthread_lock(pthread_mutex_t *mutex);
    int pthread_unlock(pthread_mutex_t *mutex);
    */    
}