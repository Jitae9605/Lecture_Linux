#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	// 입출력 파일 디스크립터 생성
	int fds1[2], fds2[2];
	char str1[] = "who are you?";
	char str2[] = "thank you for your message!";
	char buf [BUF_SIZE];
	pid_t pid;

	// 파이프 설치 및 분배
	pipe(fds1), pipe(fds2);

	// 자식프로세스 생성
	pid = fork();

	if(pid == 0)
	{	
		// 자식프로세스
		write(fds1[1], str1, sizeof(str1)); // 송신
		read(fds2[0], buf, BUF_SIZE);			// 수신
		printf("Child proc output: %s \n", buf);
	}
	else
	{
		read(fds1[0], buf, BUF_SIZE);	 		// 수신
		printf("Parent proc output: %s\n", buf);
		write(fds2[1], str2, sizeof(str2));	// 송신
		sleep(3);
	}
	return 0;
}
