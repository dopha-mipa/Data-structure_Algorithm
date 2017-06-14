/* mean heap */
#include <stdio.h>
#include <stdlib.h>

typedef struct _heap {
    int capacity;
    int size;
    int* head;
} Heap;

void insert(Heap* heap, int value);
int deleteMin(Heap* heap);
void printHeap(Heap* heap);

int main() {
  char command[5];
  int value = 0;
  int c = 31;
  int* h = (int*) malloc(sizeof(int) * c);
  int i;
  for(i = 0; i < c; i++) {
      h[i] = 0;
  }
  Heap* heap = (Heap*) malloc(sizeof(Heap));
  heap = {c, 0, h};

  while (1) {
    scanf("%s", command);

    if (!strcmp(command, "i")) {
      scanf("%d", &value);
      insert(heap, value);
    } else if (!strcmp(command, "d")) {
      printf("지워진 값은 %d\n", deleteMin(heap));
    } else if (!strcmp(command, "p")) {
      printHeap(heap);
    } else {
      break;
    }
  }
  return 0;
}

void insert(Heap* heap, int value) {
  // index는 0부터 시작한다고 하자->
  int current = heap->size;
  int parent = (current - 1) / 2;  // 부모 인덱스 찾기
  heap->head[current] = value;
  heap->size += 1;
  printf("%d\t ", heap->size);

  if (current >= heap->capacity) {
    return;
  }

  //이것도 삽입 후에 처음부터!?
  //ppt에서는 거꾸로 거슬러 오르라는데ㅜㅠ
  //부모노드랑 바꿨는데 자식노드보다 클 수도 있지 않아!?
  while(1) {
    if (parent < 0) {
      break;
    }
    if (heap->head[parent] > heap->head[current]) {
      int temp = heap->head[parent];
      heap->head[parent] = heap->head[current];
      heap->head[current] = temp;
      current = parent;
      parent = (parent - 1) / 2;
    } else {
      break;
    }
  }

  return;
}

int deleteMin(Heap* heap) {
  int min = heap->head[0];
  int current = 0;
  int left = 1;
  int right = 2;
  int smaller = (heap->head[left] < heap->head[right]) ? left : right;

  if (heap->size == 0) {
    return -1;
  }
  //마지막 원소를 루트로 데려온다
  heap->head[0] = heap->head[heap->size];
  heap->head[heap->size] = 0;
  heap->size -= 1;
  while(1) {
    if (smaller > heap->size) {
      break;
    }

    if (heap->head[current] > heap->head[smaller]) {
      int temp = heap->head[current];
      heap->head[current] = heap->head[smaller];
      heap->head[smaller] = temp;
      current = smaller;
      smaller = (
          heap->head[2*smaller + 1] < heap->head[2*smaller + 2]) ? \
            2 * smaller + 1 : 2 * smaller + 2;
    } else {
      break;
    }
  }

  return min;
}

void printHeap(Heap* heap) {
  int i = 0;
  printf("사이즈 %d ", heap->size);
  for(i = 0; i < heap->size; i++) {
    if (heap->head[i] != 0) {
      printf("%d\t ", i);
      printf("%d\t", heap->head[i]);
    }
    printf("%d\t ", i);
  }
  printf("\n");
}