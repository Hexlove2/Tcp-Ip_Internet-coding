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
