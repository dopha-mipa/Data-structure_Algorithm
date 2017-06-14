#include <stdio.h>
#include <stdlib.h>

typedef struct BT {
    int value;
    struct BT* left;
    struct BT* right;
} BT;

struct BT* bt = NULL;

void insertTreeNode(struct BT* bt, int value);
void findTreeNode(struct BT* bt, int value);
void printPre(struct BT* bt);
void printIn(struct BT* bt);
void printPost(struct BT* bt);

int main(void) {
    char command[5];
    int data;
    char p_mth[5];

    while (1) {
        printf("new loop : ");
        scanf("%s", command);

        if (!strcmp(command, "i")) {
            scanf("%d", &data);
            insertTreeNode(bt, data);
        } else if (!strcmp(command, "p")){
            scanf("%s", p_mth);
            if (!strcmp(p_mth, "in")) {
                printIn(bt);
                printf("\n");
            } else if (!strcmp(p_mth, "pre")) {
                printPre(bt);
                printf("\n");
            } else if (!strcmp(p_mth, "post")) {
                printPost(bt);
                printf("\n");
            }
        } else if (!strcmp(command, "f")) {
            scanf("%d", &data);
            findTreeNode(bt, data);
        } else {
            break;
        }
    }


    return 0;
}

void insertTreeNode(BT* bt1, int value) {
    if (bt1 == NULL) {  // 트리가 비어 있다면
      BT* newNode = (BT*) malloc (sizeof (BT));
      newNode->value = value;
      newNode->left = NULL;
      newNode->right = NULL;
      bt = newNode;
      return;
    }
    if (value >= bt1->value) { // 트리의 루트보다 값이 크다면 오른쪽으로
        if (bt1->right == NULL) { // 루트의 오른쪽이 비었다면 바로 삽입
          BT* newNode = (BT*) malloc(sizeof(BT));
          newNode->value = value;
          newNode->left = NULL;
          newNode->right = NULL;
          bt1->right = newNode;
        } else {
          insertTreeNode(bt1->right, value); // 오른쪽이 있다면 재귀적으로 삽입 실행
        }
    } else { // 트리의 루트보다 값이 작다면 왼쪽으로
        if (bt1->left == NULL) { // 왼쪽 자식이 비었다면
          BT* newNode = (BT*) malloc(sizeof(BT));
          newNode->value = value;
          newNode->left = NULL;
          newNode->right = NULL;
          bt1->left = newNode;
        } else {
            insertTreeNode(bt1->left, value); // 왼쪽 자식이 있다면
        }
    }
}

void findTreeNode(BT* bt, int value) {
    if (bt == NULL) { // 초기, 끝 노드
        printf("Tree or Key does not exist\n");
        return;
    }
    if (value > bt->value) {
        findTreeNode(bt->right, value);
    } else if (value < bt->value) {
        findTreeNode(bt->left, value);
    } else {
        printf("%p\t\t%d\n", bt, bt->value);
    }
}

void printPre(BT* bt) {
    if (bt != NULL) {
        printf("%d\t", bt->value);
        printPre(bt->left);
        printPre(bt->right);
    }
}

void printIn(BT* bt) {
    if (bt != NULL) {
        printIn(bt->left);
        printf("%d\t", bt->value);
        printIn(bt->right);
  }
}

void printPost(BT* bt) {
    if (bt != NULL) {
        printPost(bt->left);
        printPost(bt->right);
        printf("%d\t", bt->value);
  }
}
