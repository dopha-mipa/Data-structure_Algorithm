#include <stdio.h>

int cnt_5 (int num);

int main() {
  int N;
  scanf("%d", &N);

  // fct = 1;
  int num_5 = 0;
  while (N > 0) {
    // printf("%d\n", N);
    if (N % 5 == 0) {
      num_5 += cnt_5 (N);
    }
    N -= 1;
    // fct *= N;
  }
  printf("%d\n", num_5);

  return 0;
}

int cnt_5 (int num) {
  int cnt = 0;
  while (num % 5 == 0) {
    cnt += 1;
    num /= 5;
  }
  return cnt;
}