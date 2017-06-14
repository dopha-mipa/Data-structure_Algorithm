/* AVL Tree */
#include <stdio.h>
#include <stdlib.h>

typedef struct _tree {
    int value;
    int height;
    struct _tree* left;
    struct _tree* right;
} tree;

struct _tree* avl = NULL;

void insert_node (tree* node, int value);
/* 트리 루트부터 시작해서 트리의 균형을 체크하고 회전시키는 함수 */
void traverse_tree (tree* node, tree* parent, int direction);
int get_height (tree* node);
tree* find_node (tree* node, int value);
/* 회전한 포레스트와 원래 트리를 연결시켜 주는 함수 */
void where_to_insert (tree* parent, tree* to_insert, int direction);
void print_tree (tree* node);


int main() {
    int a = 0;
    insert_node (avl, 2);
    insert_node (avl, 1);
    insert_node (avl, 7);
    insert_node (avl, 5);
    insert_node (avl, 6);
    insert_node (avl, 4);
    insert_node (avl, 9);
    insert_node (avl, 8);
    insert_node (avl, 3);
    print_tree (avl);

    tree* t = find_node (avl, 7);
    a = get_height (t);
    printf("%d\n", a);

    return 0;
}

tree* find_node (tree* node, int value) {
  /* 초기, 끝 노드 */
  if (node == NULL) {
      printf ("tree or Key does not exist\n");
      return NULL;
  }
  if (value > node->value) {
      find_node (node->right, value);
  } else if (value < node->value) {
      find_node (node->left, value);
  } else {
      printf ("%p\t\t%d\n", node, node->value);
      return node;
  }
}

int get_height (tree* node) {
  int lch = 0;
  int rch = 0;
  if (node == NULL){
      return 0;
  }

  lch = get_height (node->left);
  rch = get_height (node->right);
  if (lch - rch > 0) {
      return 1 + lch;
  }

  return 1 + rch;
}

void insert_node (tree* node, int value){
  /* 이진트리의 기본 삽입 뼈대 */
  if (node == NULL) {  //트리 최초 삽입
    tree* newNode = (tree*) malloc (sizeof (tree));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    avl = newNode;
    return;
  }

  if (value >= node->value) {
    if (node->right == NULL) {
      tree* newNode = (tree*) malloc (sizeof (tree));
      newNode->value = value;
      newNode->left = NULL;
      newNode->right = NULL;
      newNode->height = 0;
      node->right = newNode;
    } else {
      insert_node (node->right, value);
    }
  } else {
    if (node->left == NULL) {
      tree* newNode = (tree*) malloc(sizeof(tree));
      newNode->value = value;
      newNode->left = NULL;
      newNode->right = NULL;
      newNode->height = 0;
      node->left = newNode;
    } else {
      insert_node(node->left, value);
    }

  }

  traverse_tree(avl, NULL, 0);
  return;
}

/* AVL의 꽃! */
void traverse_tree(tree* node, tree* parent, int direction) {
  node->height = get_height(node->left) - get_height(node->right);

  if (abs(node->height) >= 2) {
    if (node->left == NULL) {  // RR, RL
      tree* temp1 = node->right;
      if (temp1->left == NULL) { //RR
        temp1->left = node;
        node->right = NULL;
        where_to_insert(parent, temp1, direction);
      } else { //RL
        tree* temp2 = temp1->left;
        temp2->right = temp1;
        temp1->left = NULL;
        temp2->left = node;
        node->right = NULL;
        where_to_insert(parent, temp2, direction);
      }

    } else if (node->right == NULL) {  //LL, LR
      tree* temp1 = node->left;
      if (temp1->right == NULL) { //LL
        temp1->right = node;
        node->left = NULL;
        where_to_insert(parent, temp1, direction);
      } else { //LR
        tree* temp2 = temp1->right;
        temp2->left = temp1;
        temp1->right = NULL;
        temp2->right = node;
        node->left = NULL;
        where_to_insert(parent, temp2, direction);
      }

    } else {
      if (node->height >= 0) {
        traverse_tree(node->left, node, 1);
      } else {
        traverse_tree(node->right, node, -1);
      }
    }
  }
  return;

}

void where_to_insert(tree* parent, tree* to_insert, int direction) {
    if (parent == NULL) {
        avl = to_insert;
    }
    if (direction > 0) {
        parent->left = to_insert;
    } else if (direction < 0) {
        parent->right = to_insert;
    } else { // == 0
        return;
    }
}

void print_tree(tree* node) {
    if (node != NULL) {
        printf("%d\t", node->value);
        print_tree(node->left);
        print_tree(node->right);
    }
    return;
}
