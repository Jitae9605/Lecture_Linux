#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

void* thread_inc(void *arg);
void* thread_des(void *arg);

long long num = 0;

// 두 쓰레드가 같은 전역변수에 접근(=값 변경)하는데 순서가 없어서 문제가 됨
// 예를 들어, 처음 100의 전역변수가 있을때
// 값을 쓰레드1이 가져가서 101로 증가 시켰는데 
// 쓰레드2가 증가전의 값(100)을 가져가 감소시켜 99로 만들면
// 쓰레드1이 101을 반환하고 쓰레드2가 99를 이후에 반환하면
// 전역변수는  99를 가지게 되고
// 실제로 진행된 작엄은 +1-1이지만 값은 -1만 남는다!
int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	// long long 크기확인
	printf("sizeof long long : %d \n", sizeof(long long));

	for(i = 0; i < NUM_THREAD; i++)
	{
		if(i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}
	
	for(i=0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);
	
	printf("result : %lld \n", num);
	return 0;
}

void * thread_inc(void* arg)
{
	int i;
	for(i = 0; i < 50000000; i++)
		num += 1;
	return NULL;
}

void * thread_des(void * arg)
{
	int i;
	for(i = 0; i < 50000000; i++)
		num -= 1;
	return NULL;
}
