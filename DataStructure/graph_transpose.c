/**
 * 2017. 02. 06.
 * Introduction to Algorithms 22.1-3
 * 하나의 방향 그래프에 대한 전치방향 그래프의 인접 리스트와 인접 행렬 표현 두 가지 방법에 대해
 * G로부터 G^T를 계산하는 효율적인 알고리즘을 제시하고 수행시간을 분석하라.
 *
 * 인접 리스트인 경우
 * 일부 코드는 graph_adj_transform.c 에서
 *
 * 수행시간 O(V + E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct graph {
  struct list_node **graph; // 그래프를 가리키는 포인터. 인접 리스트일 경우
  int v; // 그래프 정점의 갯수
  int e; // 그래프 간선의 갯수
};

struct list_node {
  int vertex;
  struct list_node *next;
};

struct graph *transpose_graph (struct graph *g);
struct graph *matrix_to_list (int **matrix, int vertices);
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex);

int main () {
  int vertices = 6;
  int adj_mat[6][6] = {{0, 1, 0, 1, 0, 0},
                       {0, 0, 0, 0, 1, 0},
                       {0, 0, 0, 0, 1, 1},
                       {0, 1, 0, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0},
                       {0, 0, 0, 0, 0, 1}};
  int i, j;
  int **matrix = (int **) malloc (sizeof (int *) * vertices);
  for (i = 0; i < vertices; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * 6);
    memcpy (matrix[i], adj_mat[i], sizeof (int) * 6);
  }
  // 리스트로 표현된 그래프 
  struct graph *g = matrix_to_list (matrix, vertices);

  // 리스트 내용을 출력하는 코드
  struct list_node *current;
  // for (i = 0; i < vertices; i++) {
  //   printf("%d :\t", i);
  //   current = g->graph[i];
  //   while (current != NULL) {
  //     printf("%d\t", current->vertex);
  //     current = current->next; // 언제나 코드 위치는 조심합시다
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  struct graph *transposed = transpose_graph (g);

  // 전치 결과 출력
  // struct list_node *current;
  for (i = 0; i < vertices; i++) {
    printf("%d :\t", i);
    current = transposed->graph[i];
    while (current != NULL) {
      printf("%d\t", current->vertex);
      current = current->next; // 언제나 코드 위치는 조심합시다
    }
    printf("\n");
  }

  return 0;
}

struct graph *transpose_graph (struct graph *g) {
  // 결과로 반환할 그래프의 초기화
  struct graph *new_graph = (struct graph *) malloc (sizeof (struct graph));
  int vertices = g->v;
  struct list_node *current_node;
  int from;
  new_graph->graph = (struct list_node **) malloc (sizeof (struct list_node *) * vertices);
  for (from = 0; from < vertices; from++) {
    new_graph->graph[from] = NULL;
  }
  new_graph->v = vertices;
  new_graph->e = g->e;

  // 전치 연산
  for (from = 0; from < vertices; from++) {
    current_node = g->graph[from];
    while (current_node != NULL) {
      printf("from : %d, to : %d\n", from, current_node->vertex);
      struct list_node *new_node = (struct list_node *) malloc (sizeof (struct list_node));
      new_node->vertex = from;
      insert_list_node (new_graph->graph, new_node, current_node->vertex);

      current_node = current_node->next; // 빼먹지 말자 하하
    }
  }

  return new_graph;
}

struct graph *matrix_to_list (int **matrix, int vertices) {
  struct list_node **adj_list = (struct list_node **) malloc (sizeof (struct list_node *) * vertices);
  int edges = 0;
  int from, to;

  for (from = 0; from < vertices; from++) {
    for (to = 0; to < vertices; to++) {
      if (matrix[from][to] == 1) {
        struct list_node *new_node = (struct list_node *) malloc (sizeof (struct list_node));
        new_node->vertex = to;
        insert_list_node (adj_list, new_node, from);
        edges += 1;
      }
    }
  }

  struct graph *g = (struct graph *) malloc (sizeof (struct graph));
  g->graph = adj_list;
  g->v = vertices;
  g->e = edges;

  return g;
}

/**
 * graph - 그래프 
 * new_node - 해당 하는 자리에 새로 삽입될 노드
 * vertex - new_node가 삽입되어야 할 위치
 * 항상 graph[vertex]의 맨 앞에 노드를 삽입 - 정렬되지 않은 노드
 */
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex) {
  if (graph[vertex] != NULL) {
    new_node->next = graph[vertex]; // graph[vertex] = graph[vertex][0] 이라고 생각해서!
  }
  graph[vertex] = new_node;
}
