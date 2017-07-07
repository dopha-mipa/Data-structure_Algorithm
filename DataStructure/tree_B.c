/* 2017. 07. 06. 
 * B-tree
 * Nayoun Seo (puremint09@gmail.com)
 *
 * 제시하신 기준은 키(data) 1만개
 * TODO : 
 *    1. 노드에서의 삭제 연산 ;
 *    4. bool 연산값들의 처리! 이왕 있는 김에
 *    5. 중복키 확인해보기
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include "tree_B.h"

int *rand_key_generate(int size);
void unit_test();

int main() {
  int num_keys = 100;
  int* rand_keys = rand_key_generate(num_keys);

  struct b_tree *tree = tree_init();
  struct datum d = {10};
  bool success = tree_insert(tree, d);

  unit_test();

  return 0;
}

/* B-tree init */
struct b_tree *tree_init() {
  struct b_tree *tree = (struct b_tree *) malloc(sizeof(struct b_tree));
  tree->root = NULL;
  tree->num_data = 0;
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

  node->data = (struct datum *) realloc(
              node->data, sizeof(struct datum) * dim);
  node->children = (struct b_node **) realloc(
              node->children, sizeof(struct b_node *) * (dim + 1));

  return node;
}

/* Insertion must be happen at the leaf.
 * When goes down to the leaf, if cur is full then split.
 * When the leaf is full, split and add the datum to the proper node */
bool tree_insert(struct b_tree *tree, struct datum d) {
  if (tree->root == NULL || tree->num_data == 0) {  // if tree is empty
    struct b_node *new_node = realloc_node(NULL, 1);
    new_node->num_data = 1;
    new_node->data[0] = d;
    tree->root = new_node;
  }
  else {
    struct b_node *parent = NULL;
    struct b_node *cur = tree->root;
    if (check_same_key(cur, d.key)) {
      return false;
    }

    while (!check_leaf(cur)) { // 꽉 찬 부모 노드는 split
      if (check_full(cur)) {
        split(cur, parent);
      }
      parent = cur;
      cur = next_node(cur, d.key);
      if (check_same_key(cur, d.key)) {
        return false;
      }
    }

    if (check_full(cur)) { // leaf node에 대해
      split(cur, parent);
      cur = next_node(cur, d.key);
    }
    node_insert_datum(cur, d, NULL, NULL);
  }

  tree->num_data += 1;
  return true;
}

/* Check whether node is a leaf or not */
bool check_leaf(struct b_node *node) {
  bool checked = false;
  if (node->children[0] == NULL && node->children[1] == NULL) {
    checked = true;
  }
  return checked;
}

/* Check whether node is full or not */
bool check_full(struct b_node *node) {
  bool checked = false;
  if (node->num_data == MAX_KEY) {
    checked = true;
  }
  return checked;
}

/* Check whether the key is already in or not*/
bool check_same_key(struct b_node *node, int key) {
  int i;
  for (i = 0; i < node->num_data; i++) {
    if (node->data[i].key == key) {
      return true;
    }
  }
  return false;
}

/* Splits cur when cur->num_data = MAX_data */
bool split(struct b_node *cur, struct b_node *parent) {
  if (cur == NULL) {
    return false;
  }

  int middle = cur->num_data / 2;
  struct b_node *right = copy_node_half(cur, middle + 1, cur->num_data);

  if (parent == NULL) {  // if cur == root, create left
    struct b_node *left = copy_node_half(cur, 0, middle);

    cur->data[0] = cur->data[middle];
    cur = realloc_node(cur, 1);
    cur->children[0] = left;
    cur->children[1] = right;
  }
  else {  // cur != root -- cur => left
    struct b_node *recur = parent;
    struct datum go_up = cur->data[middle];
    cur = realloc_node(cur, middle);
    // TODO : 꽉 찬 부모는 여기서는 내버려두나요?
    node_insert_datum(parent, go_up, NULL, right);
  }

  return true;
}

/* WARN : Must guarantee MAX_KEY / 2 == to - from*/
struct b_node *copy_node_half(struct b_node *cur, int from, int to) {
  struct b_node *new = realloc_node(NULL, to - from);
  new->children[0] = cur->children[from];
  cur->children[from] = NULL;

  int i;
  for (i = from; i < to; i++) {
    node_insert_datum(new, cur->data[i], NULL, cur->children[i + 1]);
    // free(cur->data[i]); : 하지 않는편이 좋다고 한다. memory leak
    cur->children[i + 1] = NULL;
  }
  // TODO : 왠지 중복될거 같은데..
  cur->num_data -= (to - from);
  return new;
}

/* Find children to search next */
struct b_node *next_node(struct b_node *node, int key) {
  if (node->children[0] == NULL || node->children[1] == NULL) {
    return NULL;
  }

  int index = 0;
  while (index < node->num_data) {
    if (key < node->data[index].key) {
      break;
    }
    index += 1;
  }

  return node->children[index];  // left child of ith datum
}

/* insertion sort  
 * Guarantees node->num_data < MAX_KEY (splitted before execution) */
bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right) {
  if (node == NULL) {
    return false;
  }

  node->num_data += 1;
  int dim = node->num_data;
  node = realloc_node(node, dim);  // TODO : 불필요하다는 기분이 든다.. 조건이라도
  node->data[dim - 1] = d;  // append d to data[]

  int index = dim - 2;
  while (index > -1 && node->data[index].key > d.key) {
    node->data[index + 1] = node->data[index];
    node->children[index + 2] = node->children[index + 1];
    index -= 1; 
  }

  if (left != NULL && dim == 1) {
    node->children[index + 1] = left;
  }
  node->data[index + 1] = d;
  node->children[index + 2] = right;

  return true;
}

bool node_delete_datum(struct b_node *node, int key) {
  if (node == NULL) {
    return false;
  }

  // TODO : num_data -= 1

}

/* return array of random keys[size] */
int *rand_key_generate(int size) {
  srand(time(NULL));
  int largest = pow(2, 17);

  bool *check_repett = (bool *) malloc(sizeof(bool) * (largest + 1));
  check_repett = memset(check_repett, 0, sizeof(bool) * (largest + 1));
  // TODO : 이렇게 말고.. 좀 더 10000에 가까운 범위에서 배열을 돌며 
  // 뽑을 수 있는 알고리즘으로 써야겠다

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

void unit_test() {
  // test data tree_insert into b_node
  bool result = false;
  struct b_tree *test = tree_init();
  // root
  result = tree_insert(test, (struct datum) {5});
  result = tree_insert(test, (struct datum) {10});
  result = tree_insert(test, (struct datum) {17});
  result = tree_insert(test, (struct datum) {24});
  result = tree_insert(test, (struct datum) {30});

  // node split --- TODO !!! 왜 5가 17로 바뀌냐 ㄷㄷ
  result = tree_insert(test, (struct datum) {1});
  printf("insert {1} : %d\n", result);
}