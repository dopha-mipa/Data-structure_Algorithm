/*
 * 2017. 02. 21.
 * 소수 구하기 find Prime
 * 나윤
*/
#include <stdio.h>
#include <stdlib.h>

int is_prime (int num);

int main() {
  int M, N; // M ~ N 사이의 소수를 찾아 출력한다.
  scanf ("%d %d", &M, &N);

  int i;
  int boolean;
  for (i = M; i < N + 1; i++) {
    boolean = is_prime (i);
    if (boolean) {
      printf("%d\n", i);
    }
  }
  return 0;
}


int is_prime (int num) {
  int i;
  if (num == 1) {
    return 0;
  }
  for (i = 2; i * i <= num; i++) {
    if (num % i == 0) {
      return 0;
    }
  }
  return 1;
}