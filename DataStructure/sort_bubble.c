#include <stdio.h>

int main() {
  int arr[10] = {0, 3, 1, 8, 7, 2, 5, 4, 6, 9};
  int i, j;

  // 오름차순으로 버블 정렬
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10 - (i + 2); j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }

  for (i = 0; i < 10; i++) {
    printf("%d\t", arr[i]);
  }
  printf("\n");
}