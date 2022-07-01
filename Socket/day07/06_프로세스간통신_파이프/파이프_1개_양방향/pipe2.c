#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2];
	char str1[] = "who are you?";
	char str2[] = "thank you for your message!";
	char buf [BUF_SIZE];
	pid_t pid;

	pipe(fds);	// 파이프생성
	pid = fork();	// 자식프로세스 생성
	if(pid == 0)
	{
		// 자식프로세스
		write(fds[1], str1, sizeof(str1));	// 송신
		sleep(2); // 송신이 정상적으로 마무리 될 수 있게 잠시 대기
		read(fds[0], buf, BUF_SIZE);		// 수신
		printf("Child proc output: %s \n", buf);
	}
	else
	{
		// 부모프로세스
		read(fds[0], buf, BUF_SIZE);		// 수신
		printf("Parent proc ouutput: %s\n", buf); // 수신이 정상적으로 마무리 될 수 있게 잠시 대기
		write(fds[1], str2, sizeof(str2));	// 송신
		sleep(3);
	}
	return 0;
}


