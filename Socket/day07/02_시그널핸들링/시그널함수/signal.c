#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig == SIGALRM)
		puts("Time out!");
	alarm(2);
}

void keycontrol(int sig)
{
	if(sig == SIGINT)
		puts("CTRL + C pressed");
}

int main(int argc, char *argv[])
{
	int i;
	// signal은 모든 입력을 감시하다가 특정신호가 오면 바로 작동(멀티프로세스)
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);

	for(i = 0; i < 3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}
