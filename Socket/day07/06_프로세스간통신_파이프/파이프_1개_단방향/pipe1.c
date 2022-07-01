#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

// 1개의 파이프를 생성하고한쪽으로만 데이터가 흐름

int main(int argc, char *argv[])
{
	int fds[2];
	char str[] = "who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if(pid == 0)
	{	
		// 자식은 써서 보내기만함
		write(fds[1], str, sizeof(str));
	}
	else
	{	//부모는 읽기만 함
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}

