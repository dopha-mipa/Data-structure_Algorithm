/**
 * 방향, 가중치가 업는 그래프의 표현 변환
 * 1. matrix -> list
 * 시간 복잡도 O(V^2)...
 * 2. list -> matrix
 * TODO : 결과값이 제대로 안나온다
 * 시간 복잡도 O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct list_node {
  int vertex;
  struct list_node *next;
};

struct list_node **matrix_to_list (int **matrix, int vertices);
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex);

int **list_to_matrix (struct list_node **graph, int vertex);

int main() {
  int vertices = 6;
  int adj_mat[6][6] = {{0, 1, 0, 1, 0, 0},
                       {0, 0, 0, 0, 1, 0},
                       {0, 0, 0, 0, 1, 1},
                       {0, 1, 0, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0},
                       {0, 0, 0, 0, 0, 1}};

  /*
   * 이차원 배열 adj_mat을 그냥 쓰려고 했더니 int **과 int *[6]은 안맞는데서..
   * 한번에 프린트 잘 돌아가길래 놀랐음 ㅇㅅㅇ
   */
  int i, j;
  int **matrix = (int **) malloc (sizeof (int *) * vertices);
  for (i = 0; i < vertices; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * 6);
    memcpy (matrix[i], adj_mat[i], sizeof (int) * 6);
  }

  struct list_node **graph_list = matrix_to_list (matrix, vertices);
  // 형변환만 해줬더니 에러가 안뜨긴 하더라.. segmentation 오류 났지만 
  // struct list_node **graph_list = list_to_matrix ((int **) adj_mat, vertices);

  // 리스트 내용을 출력하는 코드
  struct list_node *current;
  for (i = 0; i < vertices; i++) {
    printf("%d :\t", i);
    current = graph_list[i];
    while (current != NULL) {
      printf("%d\t", current->vertex);
      current = current->next; // 언제나 코드 위치는 조심합시다
    }
    printf("\n");
  }

  int **graph_matrix = list_to_matrix (graph_list, vertices);

  // 행렬 내용을 출력하는 코드 - 만들어 둔 행렬과 비교

  for (i = 0; i < vertices; i++) {
    for (j = 0; j < vertices; j++) {
      if (matrix[i][j] == graph_matrix[i][j]) {
        printf("true\t");
      } else {
        printf("false\t");
      }
      // printf("%d\t", graph_matrix[i][j]);
    }
    printf("\n");
  }


  return 0;
}

struct list_node **matrix_to_list (int **matrix, int vertices) {
  struct list_node **adj_list = (struct list_node **) malloc (sizeof (struct list_node *) * vertices);
  int from, to;

  // 방향 그래프라 n^2이 될 수밖에 없나 ㅜㅠ 파이썬이었으면 좀 코드 짧았을텐데 이젠 코드도 기억 안나네
  for (from = 0; from < vertices; from++) {
    // printf("from : %d\n", from);
    for (to = 0; to < vertices; to++) {
      // printf("to : %d\n", to);
      if (matrix[from][to] == 1) {
        struct list_node *new_node = (struct list_node *) malloc (sizeof (struct list_node));
        // new_node->vertex = from;
        new_node->vertex = to;
        insert_list_node (adj_list, new_node, from);
      }
    }
  }
  return adj_list;
}

/**
 * graph - 그래프 
 * new_node - 해당 하는 자리에 새로 삽입될 노드
 * vertex - new_node가 삽입되어야 할 위치
 * 항상 graph[vertex]의 맨 앞에 노드를 삽입 - 정렬되지 않은 노드
 */
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex) {
  if (graph[vertex] != NULL) {
    // puts("\t\t\t\t들어왔댜아 - 메모리가 있을 때");
    // printf("\t\t\t\t현재 첫 노드의 vertex : %d\n", graph[vertex]->vertex);
    new_node->next = graph[vertex]; // graph[vertex] = graph[vertex][0] 이라고 생각해서!
    // TODO : 그런데 막상 graph[vertex][0]은 에러더라..ㅜㅠ
  }
  // puts("\t\t\t\t\t리스트 앞에 삽입");
  graph[vertex] = new_node;

  /* 확인용 코드
  if (new_node->next != NULL) {
    printf("\t\t\t\t원래 있던 꼭지점 : %d\n", new_node->next->vertex);
    printf("\t\t\t\t새 꼭지점 : %d\n", graph[vertex]->vertex);
  }
*/
}

int ** list_to_matrix (struct list_node **graph, int vertices) {
  int **adj_mat = (int **) malloc (sizeof (int *) * vertices);
  struct list_node *current; // 루프를 돌 변수 
  int from, to;

  // TODO - 제대로 들어오는데 오째선지 1이 다시 0이 된다.....
  for (from = 0; from < vertices; from++) {
    adj_mat[from] = (int *) malloc (sizeof (int) * vertices);
    // memset (adj_mat[from], 0, sizeof(int) * vertices);
  }
  for (from = 0; from < vertices; from++) {
    current = graph[from];

    while (current != NULL) {
      printf("들어왔다아 from : %d, to : %d\n", from, current->vertex);
      adj_mat[from][current->vertex] = 1;
      // printf("%d\n", adj_mat[from])
      printf("%d\t%d\t%d\t%d\t%d\t%d\t\n", adj_mat[from][0], adj_mat[from][1], adj_mat[from][2], 
                                           adj_mat[from][3], adj_mat[from][4], adj_mat[from][5]);
      current = current->next;
    }
  }

  return adj_mat;
}