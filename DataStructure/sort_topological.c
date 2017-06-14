/*
 * Introdunction to Algorithms
 * 깊이 우선 탐색을 이용한 위상 정렬 (Topological_sort using DFS)
 * 앞선 DFS 코드에 추가
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define V 8

struct graph {
  struct list_node **graph;
  int vertices;
  int edges;
  int time;  // DFS에 쓰일 속성
};

struct list_node { // 리스트 노드
  int vertex; // 꼭지점 이름
  struct list_node *next;
};

struct dfs_node {
  int value;; // white, grey. black 중 하나의 값을 가짐
  char wgb; // white or grey or black
  int start; // 검색된 시점
  int end; // 검색이 끝난 시점
  struct dfs_node *prev;
  struct dfs_node *next;
};

struct tpl_stack {
  struct dfs_node *sorted[V];
  int length;
};

struct graph *matrix_to_list (int **matrix, int vertices);
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex);

struct dfs_node **depth_first_search (struct graph *g, struct dfs_node **vertexes, struct tpl_stack *tpl);
void dfs_visit (struct graph *g, struct dfs_node **vertexes, int index, struct tpl_stack *tpl);
void tpl_sort (struct tpl_stack *tpl, struct dfs_node *node);

int main() {
  // 먼저 리스트를 준비해 줍시다. 
  int adj_mat[V][V] = {{0, 1, 0, 0, 1, 0, 0, 0},
                       {0, 0, 1, 0, 1, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 1},
                       {0, 0, 0, 0, 1, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 1, 0, 0, 0, 1, 0},
                       {0, 0, 0, 0, 0, 0, 0, 1},
                       {0, 0, 0, 0, 0, 0, 0, 0}};
  int i, j;
  int **matrix = (int **) malloc (sizeof (int *) * V);
  for (i = 0; i < V; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * V);
    memcpy (matrix[i], adj_mat[i], sizeof (int) * V);
  }
  // 리스트로 표현된 그래프 반환
  struct graph *g = matrix_to_list (matrix, V);

  struct dfs_node *vertexes[V];
  struct tpl_stack *tpl = (struct tpl_stack *) malloc (sizeof (struct tpl_stack));
  tpl->length = 0;
  struct dfs_node **result = depth_first_search (g, vertexes, tpl);

  for (i = V - 1; i > -1; i--) {
    printf("꼭지점 value : %d\twgb : %c\tstart : %d\tend : %d\n",
      tpl->sorted[i]->value, tpl->sorted[i]->wgb, tpl->sorted[i]->start, tpl->sorted[i]->end);
  }
  printf("\n");

  return 0;
}

// 인접 리스트를 만드는 함수들
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
  g->vertices = vertices;
  g->edges = edges;
  g->time = 0;

  return g;
}

void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex) {
  if (graph[vertex] != NULL) {
    new_node->next = graph[vertex];
  }
  graph[vertex] = new_node;
}

// DFS에 관련된 함수들
struct dfs_node **depth_first_search (struct graph *g, struct dfs_node **vertexes, struct tpl_stack *tpl) {
  int i;
  for (i = 0; i < V; i++) {
    struct dfs_node *node = (struct dfs_node *) malloc (sizeof (struct dfs_node));
    node->value = i;
    node->wgb = 'w';
    vertexes[i] = node;
  }

  for (i = 0; i < V; i++) {
    if (vertexes[i]->wgb == 'w') {
      dfs_visit (g, vertexes, i, tpl);
    }
  }
  return vertexes;
}


void dfs_visit (struct graph *g, struct dfs_node **vertexes, int index, struct tpl_stack *tpl) {
  struct dfs_node *node = vertexes[index];
  g->time += 1;
  node->start = g->time;
  node->wgb = 'g';

  struct list_node *cur = g->graph[node->value];  // index 정점과 연결된 다르 정점
  while (cur != NULL) {
    int vertex = cur->vertex;
    if (vertexes[vertex]->wgb == 'w') {
      vertexes[vertex]->prev = node;
      dfs_visit (g, vertexes, vertex, tpl);
    }
    cur = cur->next;
  }
  node->wgb = 'b';
  g->time += 1;
  // 위상 정렬
  tpl_sort (tpl, node);
  node->end = g->time;
}

void tpl_sort (struct tpl_stack *tpl, struct dfs_node *node) {
  int index = tpl->length;
  tpl->sorted[index] = node;
  tpl->length += 1;
}
