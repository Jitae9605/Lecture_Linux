#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock,char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char* argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	// 소켓생성됨(파일 디스크립터 return)
	sock = socket(PF_INET,SOCK_STREAM, 0); 
	
	// 소켓 초기화(연결할 서버정보 입력)
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	// 서버에 연결시도
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!"); //실패

	// 연결 성공하고 자식프로세스 생성_역할을 누눈다(입/출력)
	pid = fork();
	
	if(pid == 0)
	{
		// 자식프로세스동작 - 쓰기
		write_routine(sock, buf);
	}
	else
	{
		// 부모프로세스동작 - 읽기
		read_routine(sock, buf);
	}
	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1)
	{
		// 수신대기
		int str_len = read(sock, buf, BUF_SIZE);
		if(str_len == 0)
			return;

		//수신받은 메시지 출력
		buf[str_len] = 0;
		printf("Message from server : %s", buf);
	}
}

void write_routine(int sock, char *buf)
{
	while(1)
	{
		// 입력대기 및 Q,q 값 비교 감시
		fgets(buf, BUF_SIZE, stdin);
		if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
