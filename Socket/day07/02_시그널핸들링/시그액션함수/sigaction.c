#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig == SIGALRM)
		puts("Time out!");
	alarm(2);
}

// signal에서 이전에 등록한 signal 핸들러의 주소값을 받을수 있다 즉 signal + a(=연계?)의 개념
// signal을 완벽히 대체하며 오래된 프로그램의 호환성이유제외하고는 siugnal은 사양됨
int main(int argc, char*argv[])
{
	int i;
	struct sigaction act;
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM,&act, 0);

	alarm(2);

	for(i = 0; i <3 ; i++)
	{	
		puts("wait...");
		sleep(100);
	}
	return 0;
}
	
