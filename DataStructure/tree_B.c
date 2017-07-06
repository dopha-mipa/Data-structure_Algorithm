/* 2017. 07. 06. 
 * B-tree
 * Nayoun Seo (puremint09@gmail.com)
 *
 * k번째 키? 노드?의 왼쪽 자식은 k, 오른쪽 자식은 k + 1..
 *
 * 제시하신 기준은 키(data) 100만개
 * TODO : 
 *    1. header 파일로의 분리 (객체 지향 없이 짜려면 역시 핀토스 참고)
 *    2. 일단 삽입부터 차근차근
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include "tree_B.h"

int *rand_key_generate(int size);

int main() {
  int num_keys = 100;
  puts("---------------");
  int* rand_keys = rand_key_generate(num_keys);

  /*     3       7      12
   * 1 2   4 5 6   9 10    14 15 16 */

  puts("00000000000");
  struct b_node *test = realloc_node(NULL, 3);
  puts("11111111111");
  struct datum d0 = {3};
  struct datum d1 = {7};
  struct datum d2 = {12};
  // struct datum *d0 = create_datum(3);
  // struct datum *d1 = create_datum(7);
  // struct datum *d2 = create_datum(12);
  test->data[0] = d0;
  test->data[1] = d1;
  test->data[2] = d2;

  printf("keys of test(root) : %d\t%d\t%d\n", test->data[0].key, 
                          test->data[1].key, test->data[2].key);

  struct b_node *child0 = realloc_node(NULL, 2);
  struct datum d3 = {1};
  struct datum d4 = {2};
  child0->data[0] = d3;
  child0->data[1] = d4;

  struct b_node *child1 = realloc_node(NULL, 3);
  struct datum d5 = {4};
  struct datum d6 = {5};
  struct datum d7 = {6};
  child1->data[0] = d5;
  child1->data[1] = d6;
  child1->data[2] = d7;

  struct b_node *child2 = realloc_node(NULL, 2);
  struct datum d8 = {9};
  struct datum d9 = {10};
  child2->data[0] = d8;
  child2->data[1] = d9;

  struct b_node *child3= realloc_node(NULL, 3);
  struct datum d10 = {13};
  struct datum d11 = {14};
  struct datum d12 = {15};
  child3->data[0] = d10;
  child3->data[1] = d11;
  child3->data[2] = d12;

  test->children[0] = child0;
  test->children[1] = child1;
  test->children[2] = child2;
  test->children[3] = child3;


  printf("keys of test(child0) : %d\t%d\n", child0->data[0].key,
                                                child0->data[1].key);
  printf("keys of test(child1) : %d\t%d\t%d\n", child1->data[0].key,
                          child1->data[1].key, child1->data[2].key);
  printf("keys of test(child2) : %d\t%d\n", child2->data[0].key,
                                              child2->data[1].key);
  printf("keys of test(child3) : %d\t%d\t%d\n", child3->data[0].key,
                          child3->data[1].key, child3->data[2].key);


  return 0;
}

/* B-tree init */
struct b_tree *tree_init() {
  struct b_tree *tree = (struct b_tree *) malloc(sizeof(struct b_tree));
  tree->root = NULL;
  tree->num_data = 0;
  // tree->depth = -1;
  // tree->min_data = MIN_KEY;
  tree->max_order = MAX_KEY;

  return tree;
}

/* if cur == NULL, create new b_node
 * else realloc size of b_node->data and b_node->children */
struct b_node *realloc_node(struct b_node *cur, int dim) {
  struct b_node *node;
  if (cur == NULL) {
    node = (struct b_node *) malloc(sizeof(struct b_node));
    node->num_data = 0;
  }
  else {
    node = cur;
    node->num_data = dim;
  }

  // TODO : 얘네는 그냥 pointer의 array면 족하잖아.. 그럼 할당은 이거지?
  node->data = (struct datum *) realloc(
              node->data, sizeof(struct datum) * dim);
              // node->data, dim);
  node->children = (struct b_node **) realloc(
              node->children, sizeof(struct b_node *) * (dim + 1));
              // node->children, dim + 1);

  return node;
}

// struct datum *create_datum(int key) {
//   struct datum *d = (struct datum *) malloc(sizeof(struct datum));
//   d->key = key;
//   return d;
// }

// 어려운 것은 split!
/* 우선 기본 이진트리 구조로 짜고 -> B-tree에 맞게. */
bool insert(struct b_tree *tree, struct datum d) {
  if (tree->root == NULL || tree->num_data == 0) {  // if tree is empty
    struct b_node *new_node = realloc_node(NULL, 1);
    new_node->num_data = 1;
    new_node->data[0] = d;
    // new_node->depth = 0;
  }
  else {
    struct b_node *parent;
    struct b_node *cur = tree->root;
    while (!check_leaf(cur)) { // 꽉 찬 부모 노드는 split
      if (check_full(cur)) {
        // TODO
        split(cur, parent);
      }
      parent = cur;

      // TODO : cur = next(); TODO TODO TODO TODO TODO TODO
      // if (d->key )
      
    }

    if (check_full(cur)) {
      split(cur, parent);
    }
    // TODO : leaf에 key 추가하기

  }

  tree->num_data += 1;
  return false;
}

bool check_leaf(struct b_node *node) {
  bool checked = false;
  // TODO : 조건 수정 필요한지 아닌지 체크
  if (node->children[0] == NULL && node->children[1] == NULL) {
    checked = true;
  }
  return checked;
}

bool check_full(struct b_node *node) {
  bool checked = false;
  if (node->num_data == MAX_KEY) {
    checked = true;
  }
  return checked;
}

bool split(struct b_node *cur, struct b_node *parent) {
  // 루트일때, 아닐때를 잘 나눠주기
  return false;
}

/* return array of random keys[size] */
int *rand_key_generate(int size) {
  srand(time(NULL));
  int largest = pow(2, 17);

  bool *check_repett = (bool *) malloc(sizeof(bool) * (largest + 1));
  check_repett = memset(check_repett, 0, sizeof(bool) * (largest + 1));
  // TODO : NULL check?

  int *rand_arr = (int *) malloc(sizeof(int) * size);
  rand_arr = memset(rand_arr, 0, sizeof(int) * size);
  int index = 0;

  while (size > -1) {
    int temp = rand() % largest;
    while (check_repett[temp]) {  // To avoid repetition of the same key
      temp = rand();
    }

    rand_arr[index] = temp;
    check_repett[temp] = true;

    index += 1;
    size -= 1;
  }

  return rand_arr;
}