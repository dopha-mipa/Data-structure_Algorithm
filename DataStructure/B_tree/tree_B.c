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

void unit_test();
void actual_test();

int MIN_KEY;
int MAX_KEY;

int main() {
  printf("The minimum number of the data per node would be : ");
  scanf("%d", &MIN_KEY);
  MAX_KEY = 2 * MIN_KEY - 1;

  // unit_test();
  actual_test();

  return 0;
}

/* B-tree init. allocates memory */
struct b_tree *tree_init() {
  struct b_tree *tree = (struct b_tree *) malloc(sizeof(struct b_tree));
  tree->root = NULL;
  tree->num_data = 0;
  tree->min_key = MIN_KEY;
  tree->max_key = 2 * MIN_KEY - 1;

  return tree;
}

/* b_node init. allocates memory
 * WARN : node->num_key must be updated after execution T_T */
struct b_node *node_init() {
  struct b_node *node;

  node = (struct b_node *) malloc(sizeof(struct b_node));
  node->num_key = 0;
  node->data = (struct datum *) malloc(sizeof(struct datum) * MAX_KEY);
  node->data = memset(node->data, 0, sizeof(struct datum) * MAX_KEY);
  node->children = (struct b_node **) malloc(
                        sizeof(struct b_node *) * (MAX_KEY + 1));
  int i;
  for (i = 0; i < MAX_KEY + 1; i++) {
    node->children[0] = NULL;
  }

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
      cur = node_split(cur, parent);  // 수정 : 부모를 반환
      cur = tree_next_node(cur, d.key);
    }
    node_insert_datum(cur, d, NULL, NULL);
  }

  tree->num_data += 1;
  return true;
}

/* Check whether the key is already in or not */
bool node_check_same_key(struct b_node *node, int key) {
  if (node == NULL) {
    return false;
  }

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
  if (node == NULL) {
    return false;
  }

  bool checked = false;
  if (node->num_key == MAX_KEY) {
    checked = true;
  }
  return checked;
}

/* Splits cur when cur->num_key = MAX_data 
 * if cur is root of the tree, cur is still root after the execution 
 * return pointer of parent to search next node */
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
    parent = cur;  // to continue search from the parent

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
  if (cur == NULL) {
    return NULL;
  }

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
 * Guarantees node->num_key < MAX_KEY (splitted before execution) */
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
  if (tree->root == NULL || tree->num_data == 0) {
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

  bool success = node_delete_datum(cur, key, -1);
  if (success == true) {
    tree->num_data -= 1;
  }

  // if the node is empty, free the node
  if (cur->num_key == 0) {
    int index = node_find_child_index(parent, cur);  // returns -1 when cur == root

    if (index > -1 && parent != NULL) {  // if cur is not the root
      parent->children[index] = NULL;  // cur => NULL
    } else if (parent == NULL) {  // if cur is the root
      tree->root = NULL;
    }
    free(cur);
  }

  // for the case after bind : 0 root
  if (tree->root != NULL && tree->root->data[0].key == 0) {
    tree->root = tree->root->children[0];
  }
  return success;
}

/* Executed if node(hungry)->num_key < MIN_KEY while searching to delete,
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
                      kind->children[kind->num_key], hungry->children[0]);
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

  /* Since kind->num_key >= MIN_KEY was guaranted,
   * there is no needs to free the node after node_delete_datum */
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

  /* free of the unnecessary node already happens before node_delete_datum */
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
 * if known_index == -1, deletion occurs with key value
 * else deletion uses index 
 * WARN : MUST check whether the node is empty or not after execution!
 * if node is empty, please free the node */
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
    if (index < node->num_key - 1) {
      int i;
      for (i = index; i < node->num_key - 1; i++) {
        node->data[i] = node->data[i + 1];
        node->children[i] = node->children[i + 1];
      }
      datum_empty(node, i);
      node->children[i] = node->children[i + 1];
    }

    node->children[node->num_key] = NULL;
    node->num_key -= 1;
    return true;
  }
  // puts("인덱스 못 찾았다고 삭제에서?");
  return false;
}

/* Check whether node is a leaf or not */
bool node_check_leaf(struct b_node *node) {
  if (node == NULL) {
    return false;
  }

  bool checked = false;
  if (node->children[0] == NULL && node->children[1] == NULL) {
    checked = true;
  }
  return checked;
}

/* Replace old datum to {0} (Just changes the value due to memory leak) */
bool datum_empty(struct b_node *cur, int index) {
  if (cur == NULL) {
    return false;
  }

  cur->data[index] = (struct datum) {0};
  return true;
}

/* if key is in tree, return datum 
 * else return datum {0} */
struct datum tree_find_datum(struct b_tree *tree, int key) {
  if (tree->root == NULL || tree->num_data == 0) {
    return (struct datum) {0};
  }

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

/* ------------------- test ------------------- */
void unit_test() {
  // Test data tree_insert into b_node
  // struct b_tree *test = tree_init();
  // init_tree(&tree);
  struct b_tree *tree = tree_init();

  bool result = false;
  struct datum d;

  int keys = 101;
  bool success;
  int i;
  for (i = 1; i < keys; i++) {
    success = tree_insert(tree, (struct datum) {i});
  }

  print_tree(tree->root, 0);
  puts("========================");

  success = tree_delete(tree, 32);
  printf("key : 32, success : %d\n", success);
  success = tree_delete(tree, 32);
  printf("key : 32, success : %d\n", success);
  
  success = tree_delete(tree, 64);
  printf("key : 64, success : %d\n", success);
  success = tree_delete(tree, 64);
  printf("key : 64, success : %d\n", success);

  print_tree(tree->root, 0);
  puts("========================");

  for (i = 1; i < keys; i++) {
    success = tree_delete(tree, i);
    if (i % 10 == 2) {
      print_tree(tree->root, 0);
      puts("========================");
    }
  }

  success = tree_insert(tree, (struct datum) {24});
  success = tree_insert(tree, (struct datum) {36});
  success = tree_insert(tree, (struct datum) {73});
  success = tree_insert(tree, (struct datum) {11});
  success = tree_insert(tree, (struct datum) {234});
  success = tree_insert(tree, (struct datum) {2364});
  success = tree_insert(tree, (struct datum) {52});
  success = tree_insert(tree, (struct datum) {5});
  success = tree_insert(tree, (struct datum) {53});


  print_tree(tree->root, 0);
  puts("========================");

  printf("휘유\n");
}

void actual_test() {
  struct b_tree *tree = tree_init();

  FILE *fr = fopen("input_random_key_01.txt", "rt");  // 키 중복이 많다 (300000 이상)
  // FILE *fr = fopen("input_random_key_02.txt", "rt");
  // FILE *fr = fopen("input_random_key_03.txt", "rt");  // 키 중복이 적다 (10개 이하)

  if (fr == NULL) {
    puts("file open error");
    return;
  }

  int EVAL = 50000;
  int *inserted = (int *) malloc (sizeof(int) * EVAL);
  int *deleted = (int *) malloc (sizeof(int) * EVAL / 2);
  bool success;
  int i = 0;
  int j;
  int fail_cnt = 0;
  while (!feof(fr)) {
    fscanf(fr, "%d\n", &inserted[i]);
    success = tree_insert(tree, (struct datum) {inserted[i]});
    i += 1;

    struct datum found;
    if (i == EVAL) {  // 50000번째 삽입마다
      // 최근 삽입된 0 ~ 50000번 검색
      for (j = 0; j < EVAL; j++) {
        found = tree_find_datum(tree, inserted[j]);
        if (found.key != inserted[j]) {
          fail_cnt += 1;  // 검색 결과가 다르다면 fail 횟수 증가
        }
      }
      // printf("EVAL 만큼 삽입 후 트리 크기 : %d\n", tree->num_data);

      // 최근 삽입 0 ~ 50000개 중 랜덤 25000번 삭제
      srand(time(NULL));
      for (j = 0; j < EVAL / 2; j++) {
        int temp = rand() % EVAL;
        if (inserted[temp] == -1) {  // 이미 삭제된 키라면
          continue;
        }

        deleted[j / 2] = inserted[temp];
        inserted[temp] = -1;
        success = tree_delete(tree, deleted[j / 2]);  // 파일 상의 중복때문에 스킵
        found = tree_find_datum(tree, deleted[j / 2]);
        // 삭제 결과가 실패라면 - 검색 결과가 0이 아니라면
        if (found.key != 0) {
          fail_cnt += 1;
        }
      }

      // printf("  중간점검 - fail 횟수는 : %d\n", fail_cnt);
      i = 0;  // 다음 오만 개
    }

  }
  fclose(fr);

  free(inserted);
  free(deleted);

  printf("fail 횟수는 : %d\n", fail_cnt);

}