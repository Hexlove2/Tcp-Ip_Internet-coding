#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

//max of size
#define BUF_SIZE 1024

//error handle
void error_handling(char *message);


int main(int argc, char *argv[])
{
  //create
  int serv_sock, clnt_sock;

  char message[BUF_SIZE];

  int str_len, i;

  struct sockaddr_in serv_addr, clnt_addr;
  socklen_t clnt_adr_sz;

  if(argc!=2){
    printf("Usage: %s <port> \n",argv[0]);
    exit(1);
  }
  //socket()
  serv_sock=socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock==-1)
    error_handling("socket() error");
  
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  //bind()
  if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
    error_handling("bind() error");

  //listen
  if(listen(serv_sock,5)==-1)
    error_handling("listen() error");

  clnt_adr_sz = sizeof(clnt_addr);

  //accept();
  for(i=0; i<5; i++)
  {
    clnt_sock = accept(serv_sock, (struct sockaddr *)& clnt_addr, &clnt_adr_sz);
    if(clnt_sock==-1)
      error_handling("accept() error");
    else 
      printf("Connectd Client %d \n",i+1);

    while ((str_len = read(clnt_sock, message ,BUF_SIZE))!=-1) 
    {
      write(clnt_sock, message, str_len);
    }

    close(clnt_sock);
  }

  //close()
  close(serv_sock);

  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
