#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define IMG_BUF 500000

void error_handler(char *message);
char webpage[] = "HTTP/1.1 200 OK\r\n"
		"Server:Linux Web Server\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n\r\n"
		"<!DOCTYPE html>\r\n"
		"<html><head><title> My Web Page </title>\r\n"
		"<style>body {background-color: #FFFF00 }</style></head>\r\n"
		"<body><center><h1>Hello world!!</h1><br>\r\n"
		"<img src=\"lulu.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	int fdimg;
	char buf[BUF_SIZE];
	char buf2[BUF_SIZE];
	char img_buf[IMG_BUF];
	if(argc != 2)
	{
		printf(" Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handler("bind() error!");
	if(listen(serv_sock, 20) == -1)
		error_handler("listen() error!");
		
	clnt_adr_sz = sizeof(clnt_adr);
	

	while(1)
	{
		clnt_sock =
			accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
			error_handler("accept error!");
		else
			printf("Connected client\n");
		
		read(clnt_sock, buf, BUF_SIZE);
		printf("%s\n", buf);
		if(!strncmp(buf, "GET / HTTP/1.1", 14))
		{
			//puts("send HTML ~~~");
			write(clnt_sock, webpage, strlen(webpage));
		}
		else if(!strncmp(buf, "GET /lulu.jpg",13))
		{
			//puts("send lulu.jpg ~~~");
			if((fdimg = open("lulu.jpg", O_RDONLY)) == -1)
				puts("file opne err");
				
			if(read(fdimg, img_buf , sizeof(img_buf)) == -1)
				puts("file read err");

			if(write(clnt_sock, img_buf, sizeof(img_buf)) == -1);
				puts("file write err");

			close(fdimg);
		}
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handler(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
