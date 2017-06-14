/*
 * Backjoon ACMICPC 2089 - -2진수 minus binary
 * -라고 당활할 것 없이 그냥 기본 진수 구하듯 하면 된다 (예닮)
 * 아직 git이 안깔린 관계로 제출 이력은 백준에ㅜㅠ
 * 나윤
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

void minus_binary (int dividend);

int main() {
  int deci = 0;
  int remain;
  // 입력
  scanf ("%d", &deci);

  if (deci == 0 || deci == 1) {
    printf ("%d\n", deci);
    return 0;
  }

  minus_binary (deci);
  printf("\n");

  return 0;
}

void minus_binary (int dividend) {
  int quotient = dividend;
  int remain;

  if (dividend == 1) {
    printf ("%d", dividend);
    return;
  }

  quotient = (int) ((double) dividend / ((double) -2));
  // 이 음수 홀수 조건 알아내는 것이 까다로웠다 짝수를 안해봐서..ㅜㅠ
  if (dividend < 0 && dividend % -2 != 0) quotient += 1; 

  remain = dividend + (quotient * 2);

  // printf ("몫 %d\t 나머지 %d\n", quotient, remain);

  minus_binary (quotient);

  printf ("%d", remain);

  return;
}
