/* 2017. 07. 06. 
 * B-tree
 * Nayoun Seo (puremint09@gmail.com)
 *
 * 제시하신 기준은 키(data) 1만개
 * TODO : 
 *    1. 노드에서의 삭제 연산 ;
 *    4. bool 연산값들의 처리! 이왕 있는 김에
 *    5. 중복키 확인해보기
 *    6. MAX_KEY를 막쓰기 시작했다..
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
  int keys = 100;
  // int* rand_keys = rand_key_generate(keys);

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
 * else realloc size of b_node->data and b_node->children 
 * WARN : node->num_key must be updated after execution T_T */
struct b_node *node_init() {
  struct b_node *node;

  node = (struct b_node *) malloc(sizeof(struct b_node));
  node->num_key = 0;
  node->data = (struct datum *) malloc(sizeof(struct datum) * MAX_KEY);
  node->children = (struct b_node **) malloc(
                        sizeof(struct b_node *) * (MAX_KEY + 1));

  return node;
}

/* Insertion must be happen at the leaf.
 * When goes down to the leaf, if cur is full then split.
 * When the leaf is full, split it and add the datum to the proper node */
bool tree_insert(struct b_tree *tree, struct datum d) {
  if (tree->root == NULL || tree->num_data == 0) {  // if tree is empty
    struct b_node *new_node = node_init();
    new_node->num_key = 1;
    new_node->data[0] = d;
    tree->root = new_node;
  } else {
    struct b_node *parent = NULL;
    struct b_node *cur = tree->root;
    if (node_check_same_key(cur, d.key)) {
      return false;
    }

    while (!node_check_leaf(cur)) { // Goes down to the leaf (while != leaf)
      if (node_check_full(cur)) {
        node_split(cur, parent);
      }
      parent = cur;
      cur = tree_next_node(cur, d.key);
      if (node_check_same_key(cur, d.key)) {
        return false;
      }
    } // end of going down

    if (node_check_full(cur)) { // for leaf node
      node_split(cur, parent);
      if (tree->root == cur) {
        cur = tree_next_node(cur, d.key);
      } else {
        cur = tree_next_node(parent, d.key);
      }
    }
    node_insert_datum(cur, d, NULL, NULL);
  }

  tree->num_data += 1;
  return true;
}

/* Check whether node is a leaf or not */
bool node_check_leaf(struct b_node *node) {
  bool checked = false;
  if (node->children[0] == NULL && node->children[1] == NULL) {
    checked = true;
  }
  return checked;
}

/* Check whether node is full or not */
bool node_check_full(struct b_node *node) {
  bool checked = false;
  if (node->num_key == MAX_KEY) {
    checked = true;
  }
  return checked;
}

/* Check whether the key is already in or not*/
bool node_check_same_key(struct b_node *node, int key) {
  int i;
  for (i = 0; i < node->num_key; i++) {
    if (node->data[i].key == key) {
      return true;
    }
  }
  return false;
}

/* Splits cur when cur->num_key = MAX_data */
bool node_split(struct b_node *cur, struct b_node *parent) {
  if (cur == NULL) {
    return false;
  }

  int middle = cur->num_key / 2;
  struct b_node *right = node_copy_half(cur, middle + 1, cur->num_key);

  if (parent == NULL) {  // if cur == root, create left
    struct b_node *left = node_copy_half(cur, 0, middle);

    cur->data[0] = cur->data[middle];
    datum_empty(cur, middle);
    // cur = node_init(cur, 1);
    cur->num_key = 1;
    cur->children[0] = left;
    cur->children[1] = right;
  } else {  // cur != root -- cur => left
    struct datum go_up = cur->data[middle];
    cur->num_key = middle;
    datum_empty(cur, middle);
    // TODO : 꽉 찬 부모는 여기서는 내버려두나요?
    node_insert_datum(parent, go_up, NULL, right);
  }

  return true;
}

/* WARN : Must guarantee MAX_KEY / 2 == to - from*/
struct b_node *node_copy_half(struct b_node *cur, int from, int to) {
  struct b_node *new = node_init();
  new->children[0] = cur->children[from];
  cur->children[from] = NULL;

  int i;
  for (i = from; i < to; i++) {
    node_insert_datum(new, cur->data[i], NULL, cur->children[i + 1]);
    datum_empty(cur, i);  // free 등은 memory leak의 위험
    cur->children[i + 1] = NULL;
  }
  cur->num_key -= (to - from);
  return new;
}

/* Find children to search next */
struct b_node *tree_next_node(struct b_node *node, int key) {
  if (node->children[0] == NULL || node->children[1] == NULL) {
    return NULL;
  }

  int index = 0;
  while (index < node->num_key) {
    if (key < node->data[index].key) {
      break;
    }
    index += 1;
  }

  return node->children[index];  // left child of ith datum
}

/* insertion sort  
 * Guarantees node->num_key < MAX_KEY (splitted before execution) */
bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right) {
  if (node == NULL) {
    return false;
  }

  node->num_key += 1;
  int dim = node->num_key;
  node->data[dim - 1] = d;  // append d to data[]

  int index = dim - 2;  // TODO : 중복에 주의. dim과 index
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

struct datum tree_find_datum(struct b_tree *tree, int key) {
  struct b_node *cur = tree->root;
  int index = node_find_key(cur, key);
  while (index == -1 && cur != NULL) {
    cur = tree_next_node(cur, key);
    index = node_find_key(cur, key);
  }

  if (cur != NULL) {
    return cur->data[index];
  }

  return (struct datum) {0};
}

int node_find_key(struct b_node *node, int key) {
  if (node == NULL) {
    return -1;
  }
  if (node->data[node->num_key - 1].key < key || node->data[0].key > key) {
    return -1;
  }
  int index;

  int from = 0;
  int end = node->num_key;
  while (from < end) {
    int middle = from + (end - from) / 2;

    if (node->data[middle].key == key) {
      index = middle;
      break;
    } else if (node->data[middle].key < key) {
      from = middle + 1;
    } else {
      end = middle - 1;
    }
  }

  if (from >= end) {
    if (node->data[from].key == key) {
      index = from;
    } else {
      index = -1;
    }
  }

  return index;
}


bool node_delete_datum(struct b_node *node, int key) {
  if (node == NULL) {
    return false;
  }

  // TODO : num_key -= 1

}

/* Instead of free(), replace old datum to {0} (due to memory leak) */
bool datum_empty(struct b_node *cur, int index) {
  cur->data[index] = (struct datum) {0};
  return true;
}

/* return array of random keys[size] */
int *rand_key_generate(int size) {
  srand(time(NULL));
  int largest = pow(2, 17);

  bool *check_repett = (bool *) malloc(sizeof(bool) * (largest + 1));
  check_repett = memset(check_repett, 0, sizeof(bool) * (largest + 1));
  check_repett[0] = true;  // 0을 뽑는 것을 방지
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

  result = tree_insert(test, (struct datum) {5});
  result = tree_insert(test, (struct datum) {10});
  result = tree_insert(test, (struct datum) {17});
  result = tree_insert(test, (struct datum) {24});
  result = tree_insert(test, (struct datum) {30});

  struct datum d = tree_find_datum(test, 24);
  printf("d.key : %d\n", d.key);

  result = tree_insert(test, (struct datum) {1});
  result = tree_insert(test, (struct datum) {3});
  result = tree_insert(test, (struct datum) {4});

  result = tree_insert(test, (struct datum) {6});
  result = tree_insert(test, (struct datum) {7});
  result = tree_insert(test, (struct datum) {9});

  result = tree_insert(test, (struct datum) {12});
  result = tree_insert(test, (struct datum) {14});
  result = tree_insert(test, (struct datum) {16});

  result = tree_insert(test, (struct datum) {18});
  result = tree_insert(test, (struct datum) {19});
  result = tree_insert(test, (struct datum) {23});
  
  d = tree_find_datum(test, 19);
  printf("d.key : %d\n", d.key);

  result = tree_insert(test, (struct datum) {15});
  result = tree_insert(test, (struct datum) {25});
  result = tree_insert(test, (struct datum) {27});
  result = tree_insert(test, (struct datum) {28});

  result = tree_insert(test, (struct datum) {31});
  result = tree_insert(test, (struct datum) {32});
  result = tree_insert(test, (struct datum) {34});

  d = tree_find_datum(test, 27);
  printf("d.key : %d\n", d.key);

  d = tree_find_datum(test, 32);
  printf("d.key : %d\n", d.key);

  d = tree_find_datum(test, 6);
  printf("d.key : %d\n", d.key);

  d = tree_find_datum(test, 5200);
  // printf("%d\n", d);
  printf("d.key : %d\n", d.key);

}