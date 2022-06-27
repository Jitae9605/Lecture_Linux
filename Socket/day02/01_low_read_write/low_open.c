#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void error_handling(char* message);

int main(void)
{
	int fd;
	char buf[] ="Let's go!\n";

	// 빈 파일(data.txt)을 쓰기전용으로 생성(이미 존재하면 내용 지워짐)
	fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);

	if(fd == -1)
		error_handling("open() error!");
	printf("file descriptor : %d \n",fd);

	if(write(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error!");
	close(fd);
	return  0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
