#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
  int sock;
  char message[BUF_SIZE];
  struct sockaddr_in serv_addr;
  int str_len;

  if(argc!=3)
  {
    printf("Usage: %s <IP> <port>", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock==-1)
    error_handling("socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))==-1)
    error_handling("connect() error");
  else 
    puts("Connected");
  /*
  while(1)
  {
    fputs("Input messgae(Q to quit:) ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
      break;

    write(sock, message, strlen(message));
    str_len=read(sock, message, BUF_SIZE-1);
    message[str_len]=0;
    printf("messgae from server: %s", message);
  }
  */
  int str_len2,str_len3;
  while(1)
  {
    fputs("Input message(Q to quit):", stdout);
    fgets(message, BUF_SIZE, stdin);

    if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
      break;
    str_len=write(sock, message, strlen(message));
    str_len2=0;
    while(str_len2<str_len)
    {
      str_len3=read(sock, &message[str_len2], BUF_SIZE);
      if(str_len3==-1)
        error_handling("read() error");
      str_len2+=str_len3;
    }
    message[str_len]=0;
    printf("Message from server: %s", message);
  }
  close(sock);

  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n',stderr);
  exit(1);
}
