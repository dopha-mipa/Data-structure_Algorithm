/* 2017. 02. 21 
 * 소수 찾기 - find prime
 * 나윤
 */
#include <stdio.h>
#include <stdlib.h>

int is_prime (int num);

int main() {
  int l; // 주어지는 숫자의 개수
  int *arr; // 주어지는 숫자를 저장할 배열
  int count = 0; // 소수를 위한 카운트
  int r_boolean; // 1이면 소수 
  int i;

  scanf ("%d", &l);
  arr = (int *) malloc (sizeof (int) * l);
  for (i = 0; i < l; i++) {
    scanf ("%d", &arr[i]);
    r_boolean = is_prime (arr[i]);
    // printf ("수 %d\t 결과 %d\n", arr[i], r_boolean);
    if (r_boolean) {
      count += 1;
    }
  }

  printf("%d\n", count);

  return 0;
}

int is_prime (int num) {
  int i;
  if (num == 1) {
    return 0;
  }
  for (i = 2; i < num; i++) {
    if (num % i == 0) {
      return 0;
    }
  }
  return 1;
}