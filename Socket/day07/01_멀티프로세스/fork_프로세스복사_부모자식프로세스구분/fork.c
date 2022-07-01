#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char*argv[])
{
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;

	// 자식프로세스 생성(부모의 pid에는 자식의 프로세스ID가 저장(자식 :pid = 0)
	pid = fork();
	if(pid == 0)	// 자식 프로세스
		gval += 2, lval += 2;
	else		// 부모 프로세스
		gval -= 2, lval -= 2;
	
	if(pid == 0)	// 자식프로세스
		printf("Child Proc: [%d, %d] \n", gval, lval);
	else
		printf("Parent Proc: [%d, %d] \n", gval, lval);
	return 0;
}
