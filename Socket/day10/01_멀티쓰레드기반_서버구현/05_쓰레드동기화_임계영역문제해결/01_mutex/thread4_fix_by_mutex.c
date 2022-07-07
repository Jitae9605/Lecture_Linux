#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

void* thread_inc(void *arg);
void* thread_des(void *arg);

long long num = 0;

// mutex 선언(전연변수)
pthread_mutex_t mutex;

// 쓰레드의 전역변수 동시 참조문제를 mutex lock/unlock으로 해결

int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;
		
	// mutex 초기화
	pthread_mutex_init(&mutex, NULL);

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
	
	// mutex 헤제
	pthread_mutex_destroy(&mutex);
	return 0;
}

void * thread_inc(void* arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 50000000; i++)
		num += 1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void * thread_des(void * arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 50000000; i++)
		num -= 1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
