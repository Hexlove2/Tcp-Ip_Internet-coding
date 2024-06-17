#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/unistd.h>

void error_handling(char* message);

int main(int argc, char *argv[])
{
  //socket() -> bind() -> listen() ->accept() -> read()/write() -> close()
  //create socket for server and client
  int serv_sock;
  int clnt_sock;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  char message[] = "Hello World";

  //how to use
  //eg:./hello_server 8080
  if(argc != 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }
  //use tcp
  //remember to add error handle function
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1)
    error_handling("socket() error");
  

  //reset at the beginning
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;//IPV4 address group
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//get local ip and set net byte sort
  serv_addr.sin_port = htons(atoi(argv[1]));//port
  

  //start bind
  if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1)
    error_handling("bind() error");

  //start listen
  if(listen(serv_sock, 5)==-1)
    error_handling("listen() error");


  //start accept
  //clnt_sock was created atumatic when client want to connect to server
  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock=accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
  if(clnt_sock==-1)
    error_handling("accept() error");


  //use write function to send message to client
  write(clnt_sock, message, sizeof(message));
  //close all of the socket of server after sending the message 
  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(char *message)
{
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}
