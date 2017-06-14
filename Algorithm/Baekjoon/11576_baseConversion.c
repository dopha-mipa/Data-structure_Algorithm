/* 
 * 2017. 02. 21 나윤
 * Base conversion
 * 진법의 수도 10진법으로 표현된 수도 양수임이 보장됨
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void change_B (int dividend, int B);

int main() {
  int A, B; // A : 처음의 진법, B : 바꿀 진법
  int l; // 진법의 자릿수
  int *arr_A; // 각 자리의 숫자를 저장할 변수 
  int *arr_B; // 바뀐 진법의 수를 저장할 변수
  double power; // 각 자리 제곱의 수
  unsigned int sum = 0; // A진법의 수를 10진법으로 저장할 변수
  int i;

  scanf ("%d %d", &A, &B);
  scanf ("%d", &l);
  arr_A = (int *) malloc (sizeof (int) * l);
  for (i = 0; i < l; i++) {
    scanf("%d", &arr_A[i]);
    power = l - i - 1;
    sum += pow ((double) A, power) * arr_A[i];
  }

  change_B (sum, B);
  printf("\n");

  return 0;
}

void change_B (int dividend, int B) {
  unsigned int quotient = dividend;
  int remain;
  if (dividend < B) {
    printf("%d ", dividend);
    return;
  }

  quotient = dividend / B;
  remain = dividend % B;
  // printf ("몫 : %d\t 나머지 : %d\n", quotient, remain);
  change_B (quotient, B);

  printf("%d ", remain);
}

/*
예제 입력
17 8
2
2 16

예제 출력
6 2
*/