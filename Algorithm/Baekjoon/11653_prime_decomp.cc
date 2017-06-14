/* 11653 */
#include <stdio.h>
#include <math.h>

int main() {
  int N;
  scanf("%d", &N);
  int sqrt_n = sqrt(N);

  int i = 2;
  while (i < sqrt_n || N > 1) {
    if (N % i == 0) {
      printf("%d\n", i);
      N /= i;
    } else {
      i += 1;
    }
  }
  return 0;
}