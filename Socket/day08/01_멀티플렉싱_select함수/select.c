#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define 	BUF_SIZE 30

/*

 select함수
 반환값 = 타임아웃(0), 오류발생(-1), 성공(양수 == 변화발생한 파일디스크립터수)
 매개변수
  - 검사대상 파일디스크림터수(int)
  - '수신데이터 존재여부'에 관심있는 파일디스크립터 정보(fd_set *대상)
  - '블로킹 없는 데이터 전송의 가능여부'에 관심있는 파일디스크립터 정보 (fd_set *대상)
  - '예외상황 발생여부'에 관심있는 파일디스크립터 정보 (fd_set *대상)
  - select함수 호출이후 Timeout설정(timeval *대상)
*/

int main(int argc, char* argv[])
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0,&reads);	// 0 == standard input

	/*
	timeout.tv_sec = 5;
	timeout_tv_usec = 5000;
	*/

	while(1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1, &temps, 0, 0, &timeout); // select함수
		if(result == -1)
		{
			puts("select() error!");
			break;
		}
		else if(result == 0)
		{
			puts("Time-out!");
		}
		else
		{
			if(FD_ISSET(0,&temps))
			{
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from colsole : %s ", buf);
			}
		}
	}
	return 0;
}


