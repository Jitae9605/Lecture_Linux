#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* thread_main(void *arg);

int main(int argc, char* argv[])
{
	pthread_t t_id;
	int thread_parm = 5;
	void *thr_ret;
	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_parm) != 0)
	{
		puts("pthread_create() error!");
		return -1;
	};

	/*
	 전달된 스레드아이디(=첫번째 인자)에 해당하는 스레드가 종료될때까지 
	 pthread_join을 호출한 주체(프로세으 or 스레드)를 대기상태로 둔다.
	*/
	if(pthread_join(t_id, &thr_ret) != 0)
	{
		puts("pthread_join error!");
		return -1;
	};

	printf("Thread return message: %s\n", (char*)thr_ret);
	free(thr_ret);
	return 0;
}
	
void* thread_main(void *arg)
{
	int i;
	int cnt = *((int*)arg);
	char *msg = (char*)malloc(sizeof(char) * 50);
	strcpy(msg, "Hello, I'm thread! \n");

	for(i = 0; i< cnt; i++)
	{
		sleep(1);
		puts("runnig thread");
	}
	return (void*)msg;
}
