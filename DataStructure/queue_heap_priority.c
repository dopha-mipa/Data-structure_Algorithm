/**
 * 최대 힙
 * 안돌려봤다!
 * 힙 사이즈가 0일때에 대한 에러처리가 그동안 없었다는 것을 깨달았다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct heap {
  int size;
  int *arr;
};

void insert (struct heap *h, int x);
int findMaximum (struct heap *h);
int deleteMaximum (struct heap *h);
/* 원소 pre_val의 키 값을 post_val로 증가시킨다. 이때 원래의 값보다 작아지지는 않는다고 가정한다.*/
void increase_key (struct heap *h, int index, int new_val);
void check_heap (struct heap *h, int index);
void check_heap_2 (struct heap *h, int index;);

int main() {
  return 0;
}

void insert (struct heap *h, int x) {
  h->arr = (int *) realloc (arr, sizeof (int) * (h->size + 1));
  h->arr[h->size] = x;
  check_heap (struct heap *h, h->size);

  h->size += 1;
}

/**
 * 가볍게 생각해 봤지만 Introduction to Algorithms와 방향이 다른 이유는
 * 이렇게 돌면 자식-> 부모 방향으로 진행하면 모든 자식들에 대해 확인해야 하지만 
 * 책처럼 부모->자식으로 진행하며 부모를 역순으로 거슬러 올라가면 루프수가 줄어들기 때문인듯..
 * 아직 책에 나온 삽입을 구현하지는 못했지만
 * TODO : 인터넷 알고리즘 보고 구현해보기
 */
void check_heap (struct heap *h, int index) {
  int parent = (index - 1) / 2;
  if (h->arr[index] > h->arr[parent]) {  // 부모의 값보다 크다면
    int temp = h->arr[index];
    h->arr[index] = h->arr[parent];
    h->arr[parent] = temp;
    check_heap (h, parent);
  }
}

int findMaximum (struct heap *h) {
  return h->arr[0];
}

/**
 * 수행시간 O(long(n))
 * 부모-> 자식으로 내려간다.
 */
int deleteMaximum (struct heap *h) {
  int max = findMaximum (h);
  int last_index = h->size - 1;
  int last_parent = (last_size + 1) / 2;
  int parent = 0;

  /**
   * 힙 정렬때와 같이 제일 큰 수였던 루트는 제일 마지막 노드와 바꿔줌
   */
  int last_leaf = h->arr[last_index];
  h->arr[last_index] = h->arr[parent];
  h->arr[parent] = last_leaf;

  // 안해도 되는지 모르겠지만 메모리 초기화
  memset (h->arr[last_index], 0, sizeof (int));
  // 힙의 크기 하나 줄임
  h->arr = realloc (h->arr, sizeof (int) * last_index);

  /**
   * 역시 부모부터 내려가야 ㅜㅠ 그리고 뭔가 엉망이다
   * 연습문제대로 재귀가 아닌 루프로 짜면 생각보다 조건 고르는 것이 어렵다.
   * 아래에 구현한 check_heap_2 와 같은 내용.. 인데 안보고 짜서....
   */
  int left_child = 1;
  int right_child = 2;
  int bigger_child;
  do {
    bigger_child = h->arr[left_child] > h->arr[right_child] ? left_child : right_child;
    int temp = h->arr[parent];

    h->arr[parent] = h->arr[bigger_child];
    h->arr[bigger_child] = temp;

    left_child = 2 * bigger_child + 1;
    right_child = 2 * bigger_child + 2;
    parent = bigger_child;

    if (parent > last_parent) {
      break;
    } else if (parent == last_parent) { // 왼 자식만 있거나 / 자식이 없거나
      if ((last_index + 1) % 2 == 1) { // 왼 자식만 있을 경우
        right_child = left_child;
      } else { // 자식이 없음
        break;
      }
    }
  } while (h->arr[parent] < h->arr[bigger_child]);

  return max;
}

// ☆★ TODO : 키 값이 작아질 수도 있다면..?
void increase_key (struct heap *h, int index, int new_val) {
  h->arr[index] = new_val;  // 새 값으로 수정
  // // 책에서는 작아지지 않는다고 가정했지만 여기서는 
  // check_heap_2 (h, indd\ex);
  /**
   * 검사해줘야 하는 것은 부모보다 커지지는 않았나 하는 것이었다!! 8ㅁ8!!!
   * 와.. 뜻밖의 개이득... 이 함수를 쓸 일이 있었다니
   * 다만 책에서는 while을 이용해 짰다
   */
  // check_heap (h, index);
  int parent = (index - 1) / 2;
  while (index > 1 && h->arr[index] > h->arr[parent]) {
    int temp = h->arr[index];
    h->arr[index] = h->arr[parent];
    h->arr[parent] = temp;
    index = parent;
    parent = (index - 1) / 2;
  }
}

/*
 * 역시 책이 최고...
 */
void check_heap_2 (struct heap *h, int parent) {
  if (parent > (h->size + 1) / 2) break;
  int max_index = parent;
  int left_child = 2 * parent + 1;
  int right_child = 2 * parent + 2;

  if (left_child < h->size) {
    max_index = h->arr[left_child] > h->arr[max_index] ? left_child : max_index;
  }
  if (right_child < h->size) {
    max_index = h->arr[right_child] > h->arr[max_index] ? right_child : max_index;
  }

  if (max_index != parent) {
    int temp = h->arr[parent];
    h->arr[parent] = h->arr[max_index];
    h->arr[max_index] = temp;

    check_heap_2 (h, max_index);
  }
}