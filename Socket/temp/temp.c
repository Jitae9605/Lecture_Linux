 #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <fcntl.h>
 
  #define BUF 1024

 char webpage[] = "HTTP/1.1 200 OK\r\n"
       "Server:Linux Web Server\r\n"
       "Content-Type: text/html; charset=UTF-8\r\n\r\n"
       "<!DOCTYPE html>\r\n"
       "<html><head><title> My Web Page </title>\r\n"
       "<style>body {background-color: #FFFF00 }</style></head>\r\n"
       "<body><center><h1>Hello world!!</h1><br>\r\n"
       "<img src=\"test.jepg\"></center></body></html>\r\n";

 void error_handling(char *message);

 int main(int argc, char *argv[])
 {
    int serv_sock;
    int clnt_sock;

   struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
   socklen_t clnt_addr_size;
   char buf[BUF];
    char img_buf[202020];
   int n,fd,len;
 if(argc!=2)
 {
    printf("Usage : %s <port>\n",argv[0]);
   exit(1);
}

   serv_sock = socket(PF_INET, SOCK_STREAM, 0 );
   if(serv_sock ==-1)
      error_handling("socket()  error");

    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(atoi(argv[1]));

if(bind(serv_sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) ==-1)
   error_handling("bind() error");

if(listen(serv_sock, 5) ==-1)
  error_handling("accept() error");

while(1)
{
clnt_addr_size = sizeof(clnt_addr);
clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
 if(clnt_sock==-1)
    error_handling("accept() error");

 puts("Cnt");
read(clnt_sock,buf,BUF); printf("%s\n",buf);
write(clnt_sock, webpage, sizeof(webpage));

 fd = open("text.jpeg",O_RDONLY);
 read(fd,img_buf,sizeof(img_buf));
 write(clnt_sock,img_buf,sizeof(img_buf));
 close(fd);
 }

 close(clnt_sock);
close(serv_sock);
 return 0;
 }

 void error_handling (char *message)
 {
 fputs(message, stderr);
 fputc('\n', stderr);
 exit(1);
}
