#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct class {
  int no;
  unsigned int start;
  unsigned int end;
};

struct min_heap {
  int arr;
  int size;
};

int main() {
  unsigned int N = 0;
  int i;
  struct class *classes;
  struct class *rooms;
  scanf("%d", &N);
  classes = (struct class *) malloc (sizeof (struct class) * N);
  rooms = (struct class *) malloc (sizeof (struct class) * N);
  for (i = 0; i < N; i++) {
    // classes[i] = (struct class *) malloc (sizeof (struct class))
    scanf ("%d", &classes[i].no);
    scanf ("%d", &classes[i].start);
    scanf ("%d", &classes[i].end);
  }

  // 우선 순위 큐. 정렬이라면 C++이 나으려나?
  // 시작에 따라 정렬한 배열 하나, 끝나는 시간에 맡게 배열한 것 하나.


  return 0;
}

void make_min_heap (heap *h, int sort_key) {
  int parent;
  for (parent = h->size / 2; parent > -1; parent--) {
    check_heap(parent, sort_key)
  }
}

void check_heap (struct heap *h, int parent, int key) {
  if (parent > (h->size + 1) / 2) {
    return;
  }
  int min_index = parent;
  int left_chl = 2 * parent + 1;
  int right_chl = 2 * parent + 2;

  if (left_chl < h->size) {
    max_index = h->arr[left_chl] < h->arr[max_index] ? left_chl : max_index;
  }
  if (right_chl < h->size) {
    max_index = h->arr[right_chl] < h->arr[max_index] ? right_chl : max_index;
  }

  if (min_index != parent) {
    int temp = h->arr[parent];
    h->arr[parent] = h->arr[min_index];
    h->arr[min_index] = temp;

    check_heap (h, min_index);
  }
}

void insert_min_heap (struct heap *h, int x) {
  h->arr = (int *) realloc (arr, sizeof (int) * (h->size + 1));
  h->arr[h->size] = x;
  check_heap (struct heap *h, h->size);

  h->size += 1;
}