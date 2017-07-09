/* 2017. 07. 06. 
 * B-tree
 * Nayoun Seo (puremint09@gmail.com)
 *
 * (struct datum) {0} means NULL; Guarantees key < 1 does not exist
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
  // unit_test();

  struct b_tree *tree = tree_init();
  int keys = 101;
  bool success;
  int i;
  for (i = 1; i < keys; i++) {
    success = tree_insert(tree, (struct datum) {i});
  }

  // 무작위 20개 삭제
  srand(time(NULL));
  i = 0;
  while (i < 20) {
    int ind = rand() % 100 + 1;
    printf("delete key : %d, i : %d\n", ind, i);
    success = tree_delete(tree, ind);
    printf("success : %d\n", success);
    i += 1;
  } 

  print_tree(tree->root, 0);

  return 0;
}

/* B-tree init. allocates memory */
struct b_tree *tree_init() {
  struct b_tree *tree = (struct b_tree *) malloc(sizeof(struct b_tree));
  tree->root = NULL;
  tree->num_data = 0;

  return tree;
}

/* b_node init. allocates memory
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
        cur = node_split(cur, parent);
      }
      parent = cur;
      cur = tree_next_node(cur, d.key);
      if (node_check_same_key(cur, d.key)) {
        return false;
      }
    } // end of going down

    if (node_check_full(cur)) { // for leaf node
      cur = node_split(cur, parent);
      if (tree->root == cur) {
        cur = tree_next_node(cur, d.key);  // if cur == tree->root
      } else {
        cur = tree_next_node(parent, d.key);
      }
    }
    node_insert_datum(cur, d, NULL, NULL);
  }

  tree->num_data += 1;
  return true;
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

/* Check whether node is full or not */
bool node_check_full(struct b_node *node) {
  bool checked = false;
  if (node->num_key == MAX_KEY) {
    checked = true;
  }
  return checked;
}

/* Splits cur when cur->num_key = MAX_data 
 * if cur is root of the tree, cur is still root after the execution */
struct b_node *node_split(struct b_node *cur, struct b_node *parent) {
  if (cur == NULL) {
    return NULL;
  }

  int middle = cur->num_key / 2;
  struct b_node *right = node_copy_half(cur, middle + 1, cur->num_key);

  if (parent == NULL) {  // if cur == root, create left
    struct b_node *left = node_copy_half(cur, 0, middle);

    cur->data[0] = cur->data[middle];
    datum_empty(cur, middle);
    cur->num_key = 1;
    cur->children[0] = left;
    cur->children[1] = right;

    parent = cur;
  } else {  // cur != root -- cur => left
    struct datum go_up = cur->data[middle];
    cur->num_key = middle;
    datum_empty(cur, middle);

    node_insert_datum(parent, go_up, NULL, right);
  }

  return parent;
}

/* create new b_node, copy half - 1 data (from ~ to) from cur
 * WARN : Must guarantee MIN_KEY == to - from */
struct b_node *node_copy_half(struct b_node *cur, int from, int to) {
  struct b_node *new = node_init();
  new->children[0] = cur->children[from];
  cur->children[from] = NULL;

  int i;
  for (i = from; i < to; i++) {
    node_insert_datum(new, cur->data[i], NULL, cur->children[i + 1]);
    datum_empty(cur, i);
    cur->children[i + 1] = NULL;
  }
  cur->num_key -= (to - from);
  return new;
}

/* Insertion sort  
 * Guarantees node->num_key < MAX_KEY (splitted before execution)
 * Insertion occurs only when node is leaf */
bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right) {
  if (node == NULL) {
    return false;
  }

  node->num_key += 1;
  int dim = node->num_key;
  node->data[dim - 1] = d;  // append d to data[]

  int index = dim - 2;
  while (index > -1 && node->data[index].key > d.key) {
    node->data[index + 1] = node->data[index];
    node->children[index + 2] = node->children[index + 1];
    index -= 1; 
  }

  node->data[index + 1] = d;
  if (left != NULL) {
    node->children[index + 1] = left;
  }
  if (right != NULL) {
    node->children[index + 2] = right;
  }

  return true;
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

/* Deletion must be happen on the leaf.
 * if the key is place on the internal node, 
 *   find the smallest key on the right subtree, 
 *   overwrite the key with the smallest. key => smallest and continue 
 * if the node->num_key < MIN_KEY, 
 *   if the sibling->num_key >= MIN_key, borrow a key from parent 
 *   else combine with key of the parent, sibling */
bool tree_delete(struct b_tree *tree, int key) {
  if (tree->root == NULL) {
    return false;
  }

  struct b_node *parent = NULL;
  struct b_node *cur = tree->root;
  while (!node_check_leaf(cur)) {  // Goes down to the leaf
    if (cur->num_key < MIN_KEY && tree->root != cur) {
      if (!tree_borrow_key(parent, cur)) {  // if can not borrow
        cur = tree_bind_node(parent, cur);  // combine with kind sibling
      }
    }
    if (node_find_key(cur, key) != -1) {
      key = node_swap_key(cur, key);  // overwrite key with post key
      if (key == -1) {
        return false;
      }
    }
 
    parent = cur;
    cur = tree_next_node(cur, key);
  } // end going down

  if (cur->num_key < MIN_KEY && tree->root != cur) {  // meet the leaf
    if (!tree_borrow_key(parent, cur)) {  // if can not borrow
      cur = tree_bind_node(parent, cur);  // combine with kind sibling
    }
  }
  node_delete_datum(cur, key, -1);
}

/* if node(hungry)->num_key < MIN_KEY while searching to delete,
 * if sibling of node(kind)->num_key >= MIN_KEY : return true 
 * else : return false 
 * hungry->num_key < MIN_KEY, kind->num_key should be larger than MIN_KEY */
bool tree_borrow_key(struct b_node *parent, struct b_node *hungry) {
  if (hungry == NULL || parent == NULL) {
    return false;
  }

  int dim = parent->num_key;
  int index = node_find_child_index(parent, hungry);
  struct b_node *kind = NULL;

  // If child is the last pointer, use node on the left
  if (index == dim && parent->children[index - 1]->num_key >= MIN_KEY) {
    kind = parent->children[index - 1];
    /* The key would be taken from parent : data[index - 1]
     * The child would be taken from kind : children[kind->num_key]
     * The key would go up from kind to parent : kind->data[kind->num_key - 1] */
    node_insert_datum(hungry, parent->data[index - 1], 
                      kind->children[kind->num_key], NULL);
    parent->data[index - 1] = kind->data[kind->num_key - 1];
    node_delete_datum(kind, parent->data[index - 1].key, kind->num_key - 1);
  // Use node on the right
  } else if (index < dim && parent->children[index + 1]->num_key >= MIN_KEY) {
    kind = parent->children[index + 1];
    /* The key would be taken from parent : data[index]
     * The child would be taken from kind : children[0]
     * The key would go up from kind to parent : kind->data[0] */
    node_insert_datum(hungry, parent->data[index], NULL, kind->children[0]);
    parent->data[index] = kind->data[0];
    node_delete_datum(kind, parent->data[index].key, 0);
  }

  if (kind == NULL) {
    return false;
  }
  return true;
}

/* Both hungry, and kind has data less than MIN_KEY.
 * Combine two nodes with parent[index] on the middle */
struct b_node *tree_bind_node(struct b_node *parent, struct b_node *hungry) {
  if (parent == NULL || hungry == NULL) {
    return NULL;
  }

  int index = node_find_child_index(parent, hungry);
  struct b_node *kind = NULL;
  int i;
  if (index == parent->num_key) {  // if hungry is the last pointer
    kind = parent->children[index - 1];  // left sibling
    /* The key would be taken from parent : data[index - 1]
     * After execution, parent->children[index] = kind */
    node_insert_datum(kind, parent->data[index - 1], 
                              NULL, hungry->children[0]);
    for (i = 0; i < hungry->num_key; i++) {
      node_insert_datum(kind, hungry->data[i], NULL, hungry->children[i + 1]);
    }

    parent->children[index] = kind;
    free(hungry);
    node_delete_datum(parent, parent->data[index - 1].key, index - 1);
    return kind;

  // if hungry has right sibling
  } else if (index > -1 && index < parent->num_key) {
    kind = parent->children[index + 1];
    /* The key would be taken from parent : data[index]
     * After execution, parent->children[index] = hungry */
    node_insert_datum(hungry, parent->data[index], NULL, kind->children[0]);
    for (i = 0; i < kind->num_key; i++) {  // copy kind to hungry
      node_insert_datum(hungry, kind->data[i], NULL, kind->children[i + 1]);
    }

    parent->children[index + 1] = hungry;  // 순서에 주의!?
    free(kind);
    node_delete_datum(parent, parent->data[index].key, index);
    return hungry;
  }

  if (index == -1) {
    return NULL;
  }
}

/* Executed when the key is place on the internal node of the tree
 * find the smallest key (S) on the right subtree,
 * replace origin_key with S 
 * and return S to continue tree_delete(tree, S) */
int node_swap_key(struct b_node *node, int origin_key) {
  if (node == NULL) {
    return -1;
  }

  int index = node_find_key(node, origin_key);
  if (index == -1) {
    return -1;
  }

  struct b_node *cur = node->children[index + 1];
  while (!node_check_leaf(cur)) {
    cur = cur->children[0];
  }

  // Overwrites origin_key with immediate post key
  node->data[index] = cur->data[0];
  return cur->data[0].key;
}

/* Check whether datum(datum.key == key) is in the node or not, 
 * delete datum, sort back of node->data 
 * Deletion occurs only when node is leaf 
 * if known_index == -1, deletion occurs with key value
 * else deletion uses index */
bool node_delete_datum(struct b_node *node, int key, int known_index) {
  if (node == NULL) {
    return false;
  }

  int index;
  if (known_index != -1) {
    index = known_index;
  } else {
    index = node_find_key(node, key);
  }

  if (index > -1) {
    datum_empty(node, index);
    int i;
    for (i = index; i < node->num_key - 1; i++) {
      node->data[i] = node->data[i + 1];
      node->children[i] = node->children[i + 1];
    }
    datum_empty(node, i);
    node->children[i] = node->children[i + 1];
    node->children[node->num_key] = NULL;

    node->num_key -= 1; // 동작이 보장되므로 -- 식으로 써도. TODO 고려
    return true;
  }

  return false;
}

/* Check whether node is a leaf or not */
bool node_check_leaf(struct b_node *node) {
  bool checked = false;
  if (node->children[0] == NULL && node->children[1] == NULL) {
    checked = true;
  }
  return checked;
}

/* Replace old datum to {0} (Just changes the value due to memory leak) */
bool datum_empty(struct b_node *cur, int index) {
  cur->data[index] = (struct datum) {0};
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
 * else return -1 
 * Using binary search */
int node_find_key(struct b_node *node, int key) {
  if (node == NULL) {
    return -1;
  }
  if (node->data[node->num_key - 1].key < key || node->data[0].key > key) {
    return -1;
  }
  int index;

  int from = 0;
  int to = node->num_key;
  while (from < to) {  // binary search
    int middle = from + (to - from) / 2;

    if (node->data[middle].key == key) {
      index = middle;
      break;
    } else if (node->data[middle].key < key) {
      from = middle + 1;
    } else {
      to = middle - 1;
    }
  }

  if (from >= to) {
    if (node->data[from].key == key) {
      index = from;
    } else {
      index = -1;
    }
  }

  return index;
}

/* Find the index of the child among the parent->children */
int node_find_child_index(struct b_node *parent, struct b_node *child) {
  if (child == NULL || parent == NULL) {
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

bool print_tree(struct b_node *cur, int depth) {
  // struct b_node *cur = tree->root;
  if (cur == NULL) {
    return false;
  }

  int i, d;
  if (cur->children[cur->num_key] != NULL) {
    print_tree(cur->children[cur->num_key], depth + 1);
  }

  for (i = cur->num_key - 1; i > -1; i--) {
    for (d = 0; d < depth; d++) {
      printf("     ");
    }
    printf(" %d\n", cur->data[i].key);
    if (cur->children[i] != NULL) {
      print_tree(cur->children[i], depth + 1);
    }
  }

  printf("\n");
  return true;
}

/* Return array of random keys[size] - Usually raise seg fault........ */
int *rand_key_generate(int size) {
  srand(time(NULL));
  int largest = pow(2, 10);

  bool *check_repett = (bool *) malloc(sizeof(bool) * (largest + 1));
  check_repett = memset(check_repett, 0, sizeof(bool) * (largest + 1));
  check_repett[0] = true;  // prevents 0 key

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

/* unit test*/
void unit_test() {
  // Test data tree_insert into b_node
  struct b_tree *test = tree_init();
  bool result = false;
  struct datum d;

  result = tree_insert(test, (struct datum) {44});
  result = tree_insert(test, (struct datum) {57});
  result = tree_insert(test, (struct datum) {67});
  result = tree_insert(test, (struct datum) {60});
  result = tree_insert(test, (struct datum) {24});
  result = tree_insert(test, (struct datum) {5});
  result = tree_insert(test, (struct datum) {10});
  result = tree_insert(test, (struct datum) {1});
  result = tree_insert(test, (struct datum) {38});
  result = tree_insert(test, (struct datum) {3});
  result = tree_insert(test, (struct datum) {4});
  result = tree_insert(test, (struct datum) {6});
  result = tree_insert(test, (struct datum) {11});
  result = tree_insert(test, (struct datum) {8});
  result = tree_insert(test, (struct datum) {2});
  result = tree_insert(test, (struct datum) {7});
  result = tree_insert(test, (struct datum) {9});
  result = tree_insert(test, (struct datum) {12});
  result = tree_insert(test, (struct datum) {14});
  result = tree_insert(test, (struct datum) {16});
  result = tree_insert(test, (struct datum) {18});
  result = tree_insert(test, (struct datum) {19});
  result = tree_insert(test, (struct datum) {23});
  result = tree_insert(test, (struct datum) {17});
  result = tree_insert(test, (struct datum) {30});
  result = tree_insert(test, (struct datum) {15});
  result = tree_insert(test, (struct datum) {25});
  result = tree_insert(test, (struct datum) {27});
  result = tree_insert(test, (struct datum) {28});
  result = tree_insert(test, (struct datum) {31});
  result = tree_insert(test, (struct datum) {32});
  result = tree_insert(test, (struct datum) {34});
  result = tree_insert(test, (struct datum) {26});
  result = tree_insert(test, (struct datum) {35});
  result = tree_insert(test, (struct datum) {37});
  result = tree_insert(test, (struct datum) {40});
  result = tree_insert(test, (struct datum) {33});
  result = tree_insert(test, (struct datum) {41});
  result = tree_insert(test, (struct datum) {43});
  result = tree_insert(test, (struct datum) {42});
  result = tree_insert(test, (struct datum) {13});
  result = tree_insert(test, (struct datum) {20});
  result = tree_insert(test, (struct datum) {51});
  result = tree_insert(test, (struct datum) {22});
  result = tree_insert(test, (struct datum) {21});
  result = tree_insert(test, (struct datum) {45});
  result = tree_insert(test, (struct datum) {46});
  result = tree_insert(test, (struct datum) {36});
  result = tree_insert(test, (struct datum) {47});
  result = tree_insert(test, (struct datum) {61});
  result = tree_insert(test, (struct datum) {65});
  result = tree_insert(test, (struct datum) {59});
  result = tree_insert(test, (struct datum) {53});
  result = tree_insert(test, (struct datum) {49});
  result = tree_insert(test, (struct datum) {39});
  result = tree_insert(test, (struct datum) {50});
  result = tree_insert(test, (struct datum) {48});
  result = tree_insert(test, (struct datum) {62});
  result = tree_insert(test, (struct datum) {63});
  result = tree_insert(test, (struct datum) {52});
  result = tree_insert(test, (struct datum) {55});
  result = tree_insert(test, (struct datum) {56});
  result = tree_insert(test, (struct datum) {29});
  result = tree_insert(test, (struct datum) {64});
  result = tree_insert(test, (struct datum) {58});
  result = tree_insert(test, (struct datum) {66});

  print_tree(test->root, 0);

/*  // 중복키
  result = tree_insert(test, (struct datum) {22});
  result = tree_insert(test, (struct datum) {16});
  print_tree(test->root, 0);
*/
  // bind - 오른쪽 형제가 있을 때
/*  result = tree_remove(test, 44);
  result = tree_remove(test, 38);
  print_tree(test->root, 0);
  result = tree_remove(test, 30);
  print_tree(test->root, 0);*/

/*  // borrow - 왼쪽 형제를 이용해서 
  result = tree_remove(test, 67);
  print_tree(test->root, 0);*/

/*  // borrow - 오른쪽 형제를 이용해서
  result = tree_remove(test, 3);
  result = tree_remove(test, 2);
  print_tree(test->root, 0);*/

/*  // bind - 왼쪽 형제를 이용해서
  result = tree_remove(test, 44);
  print_tree(test->root, 0);
  result = tree_remove(test, 38);
  print_tree(test->root, 0);
  result = tree_remove(test, 67);
  print_tree(test->root, 0);*/

}