/**
 * Introdunction to Algorithms : 3rd Edition 에서 참고
 * index > SIZE / 2 면 리프노드라는 사실은 루프 수를 줄이는데 도움을 줍니다. ! ☆
 *
 * 시간 복잡도 O(n * log(n))
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct _heap {
  int size;
  int *arr;
} heap;

#define SIZE 20 

void heap_sort (heap *h);
void make_heap (heap *h);
void check_heap (heap *h, int parent);

int main() {
  // 초기 선언
  int a[SIZE] = {10, 3, 11, 8, 17, 2, 5, 14, 6, 19, 
                  0, 7, 4, 9, 1, 18, 12, 16, 15, 13};
  int i = 0;
  heap *h = (heap *) malloc (sizeof (heap));
  h->size = 20;
  h->arr = a;

  // 힙 정렬
  heap_sort (h);

  // 출력
  for (i = 0; i < SIZE; i++) {
    printf("%d\t", h->arr[i]);
  }
  printf("\n");

  return 0;
}

void heap_sort (heap *h) {
  int size = h->size;
  int i;

  make_heap (h); // 배열을 힙으로 구성

  // 힙의 최대값 (루트) 를 힙의 뒤부터 정렬하고 앞 부분은 다시 힙으로 구성
  for (i = h->size - 1; i > -1; i--) {
    int temp = h->arr[0];
    h->arr[0] = h->arr[i];
    h->arr[i] = temp;

    h->size -= 1;
    check_heap (h, 0);
  }
  h->size = size;
}

void make_heap (heap *h) {
  int parent;
  int i;
  // 최대 힙으로 만듦
  for (parent = h->size / 2; parent > -1; parent--) {
    check_heap (h, parent);
  }
}

void check_heap (heap *h, int parent) {
  int max_index = parent;
  int left_child = 2 * parent + 1;
  int right_child = 2 * parent + 2;
  if (parent > h->size / 2) { //※ 인덱스 > (SIZE / 2) 이면 리프 노드이다.
    return;
  }

  if (left_child < h->size) {
    max_index = h->arr[left_child] > h->arr[parent] ? left_child : parent;
  }
  if (right_child < h->size) {
    max_index = h->arr[right_child] > h->arr[max_index] ? right_child : max_index;
  }

  if (max_index != parent) {
    int temp = h->arr[max_index];
    h->arr[max_index] = h->arr[parent];
    h->arr[parent] = temp;

    check_heap (h, max_index);
  }
}
