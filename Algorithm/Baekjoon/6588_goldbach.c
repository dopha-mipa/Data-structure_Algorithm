/*
 * 2017. 02. 21.
 * 골드바흐의 추측 - Goldbach's Conjecture
 * 나윤
 * 1,000,000 이하의 모든 짝수에 대해서
 * 4보다 큰 짝수는 두 홀수 소수의 합으로 나타낼 수 있다 검증하기
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int eratosthenes_sieve (int* num, int *prime, int max);
int goldbach_conjecture (int *num, int *prime, int prime_count, int input);

int main() {
  int input = 1;
  // int count = 0;

  int MAX = 1000001; // 0 ~ 백만
  int *num = (int *) malloc (sizeof (int) * MAX); // 0 ~ 백만의 숫자 배열
  // int *prime = (int *) malloc (sizeof (int)); // 찾은 홀수 소수의 배열
  int prime[78498] = {0, };
  int prime_count = 0; // 찾은 소수의 수
  int is_success;
  int i;

  memset (num, 1, sizeof (int) * MAX); // num을 1으로 초기화 

  prime_count = eratosthenes_sieve (num, prime, MAX); // 먼저 소수 정보 등록

  while (1) {
    scanf ("%d", &input);
    if (input == 0) {
      break;
    }

    is_success = goldbach_conjecture (num, prime, prime_count, input);
    if (is_success == 0) {
      printf ("Goldbach's conjecture is wrong.\n");
    }
  }
/*
  printf ("prime_count %d\n", prime_count);
  printf ("sizeof int %lu\n", sizeof (int));
  printf ("sizeof prime %lu\n", sizeof (prime));
  for (i = 0; i < prime_count; i++) {
    printf ("1");
  }
  printf ("\n끝\n");
*/
  free (num);
  // ?? TODO : 왜일까요ㅜㅠ
  // prime = (int *) realloc (prime, sizeof (int));
  // free (prime);

  return 0;
}

/* 
 * num : max까지 숫자의 배열의 주소. 1로 초기화되어 있음
 * prime : 찾은 소수를 저장할 배열의 주소 
 * max : 에라토스테네스의 체 적용 범위 + 1 (0 인덱스 포함)
 * 반환 : 찾은 소수의 갯수
 */
int eratosthenes_sieve (int* num, int *prime, int max) {
  int prime_count = 0; // 반환값
  int i, j;

  num[0] = 0;
  num[1] = 0;

  for (i = 2; i < max; i++) {
    if (num[i]) { // 아직 지워지지 않은 수라면 소수에 등록한다.
      // prime = (int *) realloc (prime, sizeof (int) * prime_count + 1);
      prime[prime_count] = i; 
      prime_count += 1;
      for (j = 2 * i; j < max; j += i) {
        num[j] = 0;
      }
    }
  }

  return prime_count;
}

/*
 * num : max까지 숫자의 배열의 주소. 1로 초기화되어 있음
 * prime : 찾은 소수를 저장할 배열의 주소 
 * input : 분해할 수
 * 반환값 : 인풋을 분해할 소수를 찾았는지의 여부
 */
int goldbach_conjecture (int *num, int *prime, int prime_count, int input) {
  int i;
  int b_prime;
  int is_success = 0;
  for (i = 0; i < prime_count; i++) {
    int b_prime = input - prime[i];
    if (num[b_prime]) {
      printf ("%d = %d + %d\n", input, prime[i], b_prime);
      is_success = 1;
      break;
    }
  }
  return is_success;
}