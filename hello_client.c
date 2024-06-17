#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/unistd.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
  //remember to add error handling function after every socket function
  //socket() -> connect() -> read()/write() ->close()
  //reset at the beginning
  //how to use: eg: ./client 127.0.0.1 8080
  int sock;
  struct sockaddr_in serv_addr;
  char message[30];
  int str_len;
  
  //Usage
  if(argc!=3)
  {
    printf("Usage: %s <ip> <port>",argv[0]);
    exit(1);
  }
  
  //socket
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock == -1)
    error_handling("socket() error");

  //set server ip address
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  //Attention: In this example below, we should use inet_addr to change string to int
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_addr.sin_port=htons(atoi(argv[2]));

  //connect
  if(connect(sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1)
    error_handling("connect() error");
  
  //read
  str_len=read(sock, message, sizeof(message)-1);
  if(str_len==-1)
    error_handling("read() error");

  //display the message from server
  printf("Message from server: %s \n",message);

  //close
  close(sock);

  return 0;
}
void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n',stderr);
  exit(1);
}
