/**
 * Introdunction to Algorithms : 3rd Edition 에서 참고
 * 시간 복잡도 O(n * log(n)) (평균)
 * 분할이 일정한 비율로 이루어지는 경우에도 n * log(n)
 * TODO : 예외 처리, 랜덤 퀵 구현
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define SIZE 10

void quick_sort (int *arr, int from, int to);
int quick_partition (int *arr, int from, int to);

int main() {
  int arr[SIZE] = {0, 3, 1, 8, 7, 2, 5, 4, 6, 9};
  int i;

  quick_sort (arr, 0, 9);

  for (i = 0; i < SIZE; i++) {
    printf("%d\t", arr[i]);
  }
  printf("\n");

  return 0;
}

void quick_sort (int *arr, int from, int to) {
  if (from < to) {
    int mark = quick_partition (arr, from, to); // mark는 이미 정렬된 상태이다.
    quick_sort (arr, from, mark - 1);
    quick_sort (arr, mark + 1, to);
  }
}

/*
 * quick의 꽃!
 * 이해시켜준 예닮이 감사
 * 부분 배열 from ~ to 에서의 수행 시간은 n = to - from + 1 일 때 O(n)
 * 성능이 가장 나쁠 때 : n-1 / 0 개의 집합으로 나눠질 때. 
 * O(n)짜리 연산을 n번 해야 하므로 n^2이 된다!
 */
int quick_partition (int *arr, int from, int to) {
  int end_value = arr[to];
  int i, j;
  int temp;

  for (i = from - 1, j = from; j < to; j++) {
    if (arr[j] <= end_value) {
      i += 1; // i를 기준으로 end_value보다 큰 값, 작은 값이 나뉜다.
      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }

  if (i + 1 > to) { // 모든 원소가 같을 때
    return (from + to) / 2; // 책에서 원한 것. 하긴 배열을 반환할 수는 없으니까 ㅜㅠ
    // 그래도 이미 정렬되어있기는 한건데 아깝다..
  }

  i += 1;
  temp = arr[i];
  arr[i] = arr[to];
  arr[to] = temp;

  return i;
}

int randomized_quick_partition (int *arr, int from, int to) {
  srand(time(NULL));
  int mark = rand() % (to - from + 1) + from - 1;

  int temp = arr[mark];
  arr[mark] = arr[to];
  arr[to] = temp;

  return quick_partition (arr, from, to);
}