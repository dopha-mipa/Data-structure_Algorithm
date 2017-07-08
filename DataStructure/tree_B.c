/* 2017. 07. 06. 
 * B-tree
 * Nayoun Seo (puremint09@gmail.com)
 *
 * 제시하신 기준은 키(data) 1만개
 * TODO : 
 *    4. bool 연산값들의 처리! 이왕 있는 김에 - 이런 데서 코드 길이가 길어지나?
 *    5. 중복키 확인해보기 (unit test);
 *    6. YD : 주석을 더 잘 달자. 라고 추천해줌
 *    7. 아 search key 관련 메소드에서도 이진 탐색 하도록
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
  } else {   // if tree is not empty
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

/* Check whether the key is already in or not */
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

/* WARN : Must guarantee MIN_KEY == to - from */
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

/* Insertion sort  
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

  node->data[index + 1] = d;
  // TODO : 한차례 커밋에 코멘트를 남겨둠.. 이건 나의 혜안이었던 거시다..
  // 새로 추가하는 키가 제일 앞에 갈 거라면.. 필요한건 left가 될 수 도 있..
  // 은 개뿔 뭔가 이유가 있어서 썼었는데 기억이 안난다...ㅜㅠㅜㅠ
  if (left != NULL) {
    node->children[index + 1] = left;
  }
  if (right != NULL) {
    node->children[index + 2] = right;
  }

  return true;
}

/* if key is in tree, return datum 
 * else return datum {0} */
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

/* if key is in b_node, return index of datum in node->data 
 * else return -1 */
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

/* TODO : Unit Test ------------------------------------------------------- */

bool tree_remove(struct b_tree *tree, int key) {
  if (tree->root == NULL) {
    return false;
  }
  // TODO : 루트면.. swap()만 하는듯. 아하
  struct b_node *parent = NULL;
  struct b_node *cur = tree->root;
  while (!node_check_leaf(cur) && tree->root != cur) {
    if (cur->num_key <= MIN_KEY) {
      if (!tree_borrow_key(parent, cur)) {
        tree_bind_node(parent, cur);
      }
    }
    if (node_find_key(cur, key)) {
      /* 키를 오륹쪽 leaf의 제일 작은 값으로 바꿔준다  swap*/
      /* 그리고는 사실 바로 parent랑 자식 노드 확보해서 하면 되지 않나..?*/
      /* 최소 부모노드만 확보해도 뭐 */
    }
 
    parent = cur;
    tree_next_node(cur, key);
  }

  if (cur->num_key <= MIN_KEY && tree->root != cur) {
    if (!tree_borrow_key(parent, cur)) {
        tree_bind_node(parent, cur);
    }
    /*삭제하기*/
  }
}

/* if node(hungry)->num_key <= MIN_KEY while searching to delete,
 * if sibling of node(kind)->num_key > MIN_KEY : return true 
 * else : return false 
 * hungry->num_key <= MIN_KEY, kind->num_key should be larger than MIN_KEY */
bool tree_borrow_key(struct b_node *parent, struct b_node *hungry) {
  if (hungry == NULL || parent == NULL) {
    return false;
  }

  int dim = parent->num_key;
  int index = node_find_child_index(parent, hungry);
  struct b_node *kind;

  // If child is the last pointer, use node on the left
  if (index == dim && parent->children[index - 1]->num_key > MIN_KEY) {
    kind = parent->children[index - 1];
    /* The key would be taken from parent : data[index - 1]
     * The child would be taken from kind : children[kind->num_key]
     * The key would go up from kind to parent : kind->data[kind->num_key - 1] */
    node_insert_datum(hungry, parent->data[index - 1], 
                      kind->children[kind->num_key], NULL);
    parent->data[index - 1] = kind->data[kind->num_key - 1];
    node_delete_datum(kind, kind->num_key - 1);
  // Use node on the right
  } else if (index < dim && parent->children[index + 1]->num_key > MIN_KEY) {
    kind = parent->children[index + 1];
    /* The key would be taken from parent : data[index]
     * The child would be taken from kind : children[0]
     * The key would go up from kind to parent : kind->data[0] */
    node_insert_datum(hungry, parent->data[index], NULL, kind->children[0]);
    parent->data[index] = kind->data[0];
    node_delete_datum(kind, 0);
  }

  if (kind == NULL) {
    return false;
  }
  return true;
}

bool tree_bind_node(struct b_node *parent, struct b_node *hungry) {
  if (parent == NULL || hungry == NULL) {
    return false;
  }

  int index = node_find_child_index(parent, hungry);
  struct b_node *kind;
  int i;
  if (index == parent->num_key) {  // if hungry is the last pointer
    kind = parent->children[index - 1];
    /* The key would be taken from parent : data[index - 1]
     * After execution, parent->children[index] = kind */
    node_insert_datum(kind, parent->data[index - 1], 
                              NULL, hungry->children[0]);
    for (i = 0; i < hungry->num_key; i++) {
      node_insert_datum(kind, hungry->data[i], NULL, hungry->children[i + 1]);
    }

    free(hungry); // TODO : ? 없어지는 거니까 맞지 않을까?
    node_delete_datum(parent, index - 1);

  // if hungry has right sibling
  } else if (index > -1 && index < parent->num_key) {
    kind = parent->children[index + 1];
    /* The key would be taken from parent : data[index]
     * After execution, parent->children[index] = hungry */
    node_insert_datum(hungry, parent->data[index], NULL, kind->children[0]);
    for (i = 0; i < kind->num_key; i++) {  // copy kind to hungry
      node_insert_datum(hungry, kind->data[i], NULL, kind->children[i + 1]);
    }

    free(kind); // TODO : ? 없어지는 거니까 맞지 않을까?
    parent->children[index + 1] = hungry;
    node_delete_datum(parent, index);
  }

  if (index == -1) {
    return false;
  }
  return true;
}

/* 삭제해야할 키가 내부노드일때, 오른쪽 트리의 가장 작은 값을 리턴해주는 거시지 */
bool tree_swap_key(struct b_node *node, int key) {
  if (node == NULL) {
    return false;
  }

  int index = node_find_key(node, key);
  if (index == -1) {
    return false;
  }

  struct b_node *cur = node->children[index + 1];
  while (!node_check_leaf(cur)) {
    cur = cur->children[0];
  }

  struct datum temp = node->data[index];
  node->data[index] = cur->data[0];
  cur->data[0] = temp;

  return true;
}

/* Find the index of the child among the parent->children */
int node_find_child_index(struct b_node *parent, struct b_node *child) {
  if (child == NULL || parent == NULL) {  // 없어도 될 것 같지만 앞 일을 몰라서
    return -1;
  }

  int index;  // To find index of hungry among parent->children
  for (index = 0; index < parent->num_key + 1; index++) {
    if (parent->children[index] == child) {
      break;
    }
  }

  if (index > parent->num_key) {
    index = -1;
  }
  return index;
}

/* Check whether datum(datum.key == key) is in the node or not, 
 * delete datum, sort back of node->data */
bool node_delete_datum(struct b_node *node, int key) {
  if (node == NULL) {
    return false;
  }

  int index = node_find_key(node, key);
  if (index > -1) {
    datum_empty(node, index);
    int i;
    for (i = index; i < node->num_key - 1; i++) {
      node->data[i] = node->data[i + 1];
      node->children[i] = node->children[i + 1];
    }

    node->children[node->num_key] = NULL;
    node->num_key -= 1; // 동작이 보장되므로 -- 식으로 써도. TODO 고려
    return true;
  }

  return false;
}

/* Replace old datum to {0} (due to memory leak) */
bool datum_empty(struct b_node *cur, int index) {
  cur->data[index] = (struct datum) {0};
  return true;
}

/* Return array of random keys[size] */
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
  // Test data tree_insert into b_node
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


  tree_swap_key(test->root, 12);
  printf("휘유");

}