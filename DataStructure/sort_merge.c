/**
 * 병합 정렬
 * 1. 정렬한 데이터 집합을 반으로 나눈다.
 * 2. 나뉜 하위 데이터 집합의 크기가 2 이상이면 1을 반복한다.
 * 3. 하위 데이터 집합 둘을 비교하며 하나의 집합으로 정렬한다.
 * 4. 데이터 집합이 하나가 될 때까지 3을 반복한다. 
 * 2017. 01. 24. 완료
 *
 * 시간 복잡도 O(n * log(n))
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
// #define SIZE 9
// #define SIZE 18
#define SIZE 2

void merge_sort (int* arr, int from, int to);
void sort_sets (int* arr, int from, int middle, int to);

int main () {
  // int a[SIZE] = {5, 1, 6, 4, 8, 3, 7, 9, 2};
  // int a[SIZE] = {5, 12, 1, 14, 6, 10, 6, 11, 4, 5, 8, 15, 6, 13, 3, 7, 9, 2};
  int a[SIZE] = {5, 1};
  int i = 0;

  merge_sort(a, 0, SIZE - 1);

  for (i = 0; i < SIZE; i++) {
    printf("%d\t", a[i]);
  }
  printf("\n");
}

/**
 * 데이터 집합의 크기가 1이 될 때까지 나누기
 * from, to는 데이터 집합 하나의 시작과 끝 번호
 */
void merge_sort (int* arr, int from, int to) {
  if (to - from  > 1) {
    int middle = (from + to) / 2;
    merge_sort (arr, from, middle);
    merge_sort (arr, middle + 1, to);

    // TODO : 병합을 위한 함수 호출? middle을 어떻게 이용할 수 있을 것인갸 ;ㅅ;
    sort_sets(arr, from, middle, to);
    return;
  }

  // dataSet의 크기가 2일 때 (to - from = 1), 1일 때
  if (arr[from] > arr[to]) {
    int temp = arr[from];
    arr[from] = arr[to];
    arr[to] = temp;
  }
}

/**
 * 나눠진 데이터 집합을 정렬하여 병합
 */
void sort_sets (int* arr, int from, int middle, int to) {
  int *new_arr;  // 정렬 결과를 임시 저장할 집합
  int left = from; // 왼쪽 데이터 집합의 시작 인덱스
  int right = middle + 1; // 오른쪽 데이터 집합의 시작 인덱스
  int i = 0;

  new_arr = (int *) malloc (sizeof(int) * (to - from + 1));
  memset(new_arr, 0, sizeof(int) * (to - from + 1));

  while (left <= middle && right <= to) {
    if (arr[left] < arr[right]) {
      new_arr[i] = arr[left];
      left += 1;
    } else {
      new_arr[i] = arr[right];
      right += 1;
    }
    i += 1;
  }

  if (right <= to) { // 오른쪽 데이터 집합의 원소가 남았다면
    memcpy(new_arr + i, arr + right, sizeof(int) * (to - right + 1));
  } else {
    memcpy(new_arr + i, arr + left, sizeof(int) * (middle - left + 1));
  }

  memcpy(arr + from, new_arr, sizeof(int) * (to - from + 1));
}