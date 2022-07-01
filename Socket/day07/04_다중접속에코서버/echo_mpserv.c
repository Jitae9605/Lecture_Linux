#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 본래 1명의 클라이언트 밖에 받지 못하던것을 멀티프로세스를 이용해 한꺼번에 처리한다.
// 즉, 부모는 접속만 대기, 클라이언트가 오면 자식을 만들어서 배정(1:1), 클라이언트 가면 자식 삭제
#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, status;
	char buf[BUF_SIZE];
	if(argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// sigaction 초기화
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	// 서버초기화 및 sigaction 등록(자식프로세스 생성)
	status = sigaction(SIGCHLD, &act, 0);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	// 접속대기
	while(1)
	{	// 클라이언트 주소크기 등록
		adr_sz = sizeof(clnt_adr);
		
		// 클라이언트의 소켓을 허용 및 등록
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock == -1)	// 접속이 없으면 -1 반환
			continue;
		else
			puts("new clinet conneted..."); // 클라이언트 접속
			
		// 자식프로세서 생성	
		pid = fork();
		if(pid == -1)
		{	// 프로세서 동작 실패 -> 다시 접속 대기
			close(clnt_sock);
			continue;
		}
		if(pid == 0)
		{	// 자식프로세서(= 클라이언트와 1:1 연결)
			close(serv_sock); // 서버소켓 닫기
			
			// 에코서버이므로 입력받은 내용을 다시 출력한다
			// 클라이언트 소켓내에 buf가있으면(= 수신내용) 이를 읽어온다
			while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len); // 읽어온 내용 송신버퍼에 출력

			close(clnt_sock);
			puts("client disconneted...");
			return 0;
		}
		else// 부모프로세서 
			close(clnt_sock); // 클라이언트 소켓 삭제
	}
	// 끝났으므로 부모프로세서도 남은 서버 소켓도 삭제
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG); // pid 종료를 기다림(상대 연결 끊어짐을 대기)
	printf("remove proc id : %d \n",pid);
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
