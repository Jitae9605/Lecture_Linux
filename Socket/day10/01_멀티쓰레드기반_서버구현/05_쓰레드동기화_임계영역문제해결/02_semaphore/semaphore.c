#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void *arg);
void * accu(void *arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

// 세마포어는 키(0,1)로 작동하는 원리
// sem_init = 세마포어 오브젝트생성
//				  (각 오브젝트는 오직 0, 1만을 가지고있고 가질수 있다)
// sem_post가 호출되면 대상의 오브젝트값은 1이 된다.
// sem_wait가 호출되면 대상의 오브젝트값은 0이 된다.
// 오브젝트값이 1이 아니면 임계영역으로의 쓰레드 진입이 허용되지 않는다.

// 즉, 쓰레드들에게 함수의 우선순위(순서)를 부여하는 것!

int main(int argc, char* argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void * read(void * arg)
{
	int i;
	for(i = 0; i < 5; i++)
	{
		fputs("Iput num : ", stdout);

		sem_wait(&sem_two); // 임계영역시작
		scanf("%d", &num);
		sem_post(&sem_one); // 임계영역끝
	}
	return NULL;
}

void * accu(void *arg)
{
	int sum = 0, i;
	for(i = 0; i < 5; i++)
	{
		sem_wait(&sem_one); // 임계영역 시작
		sum += num;
		sem_post(&sem_two); // 임계영역 끝
	}
	printf("Result : %d\n", sum);
	return NULL;
}
		
