#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	pid_t  pid = fork();
	if(pid == 0)	// childprocess인 경우, 
	{
		puts("Hi, I am a child process");
	}
	else
	{
		printf("Child Process ID: %d \n",pid);
		sleep(30);
	}

	if(pid == 0)
		puts("End child process");
	else
		puts("End parent process");
	return 0;
}
