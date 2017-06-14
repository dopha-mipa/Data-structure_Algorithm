/*
 * Introduction to Algorithms
 * 2017. 02. 14.
 * Strong-connected Components
 * 그래프 G와 그 전치 그래프 G'에 대해 DFS를 각각 수행한다
 * end 시간의 감소 순으로 정점을 고려하기 위해 topological.c와 graph_transpose.c 이용
 *
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

struct scc_tree {
  struct dfs_node **tree;
  int components;
};

struct graph *matrix_to_list (int **matrix, int vertices);
struct graph *transpose_graph (struct graph *g);
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex);

struct dfs_node **depth_first_search (struct graph *g, struct dfs_node **vertexes, struct tpl_stack *tpl);
void dfs_visit (struct graph *g, struct dfs_node **vertexes, int index, struct tpl_stack *tpl);
void tpl_sort (struct tpl_stack *tpl, struct dfs_node *node);

struct scc_tree *depth_first_search_on_T (struct graph *g, struct dfs_node **tpl_sorted);
struct dfs_node *dfs_visit_on_T (struct graph *gT, struct dfs_node **tpl_sorted, int index);

int main() {
  // 먼저 리스트를 준비해 줍시다. 
  int adj_mat[V][V] = {{0, 1, 0, 0, 0, 0, 0, 0},
                       {0, 0, 1, 0, 1, 1, 0, 0},
                       {0, 0, 0, 1, 0, 0, 1, 0},
                       {0, 0, 1, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 1, 0},
                       {0, 0, 0, 0, 0, 1, 0, 0},
                       {0, 0, 0, 1, 0, 0, 0, 0}};
  int i, j;
  int **matrix = (int **) malloc (sizeof (int *) * V);
  for (i = 0; i < V; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * V);
    memcpy (matrix[i], adj_mat[i], sizeof (int) * V);
  }
  // 리스트로 표현된 그래프 반환
  struct graph *g = matrix_to_list (matrix, V);

  struct dfs_node *vertexes[V];
  vertexes[0] = NULL;
  struct tpl_stack *tpl = (struct tpl_stack *) malloc (sizeof (struct tpl_stack));
  tpl->length = 0;
  
  depth_first_search (g, vertexes, tpl);
 //위상 정렬 결과를 확인하는 코드
  for (i = V - 1; i > -1; i--) {
    printf("꼭지점 value : %d\twgb : %c\tstart : %d\tend : %d \n",
      tpl->sorted[i]->value, tpl->sorted[i]->wgb, tpl->sorted[i]->start, tpl->sorted[i]->end);
  }
  printf("\n");


  struct graph *gT = transpose_graph (g);
  struct list_node *cur_list;
 //전치시킨 그래프의 인접리스트를 확인하는 코드
  for (i = 0; i < V; i++) {
    cur_list = gT->graph[i];
    printf("value : %d", i);
    while (cur_list != NULL) {
      printf("\t%d", cur_list->vertex);
      cur_list = cur_list->next;
    }
    printf("\n");
  }


  struct scc_tree *scc = depth_first_search_on_T (gT, tpl->sorted); // 전치한 그래프를 넣자..!^^


  struct dfs_node *cur_dfs;
  printf("component : %d\n", scc->components);
  for (i = 0; i < scc->components; i++) {
    cur_dfs = scc->tree[i];
    while (cur_dfs != NULL) {
      printf("꼭지점 : %d\t", cur_dfs->value);
      cur_dfs = cur_dfs->next;
    }
    printf("\n");
  }

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

struct graph *transpose_graph (struct graph *g) {
  // 결과로 반환할 그래프의 초기화
  struct graph *new_graph = (struct graph *) malloc (sizeof (struct graph));
  int vertices = g->vertices;
  struct list_node *current_node;
  int from;
  new_graph->graph = (struct list_node **) malloc (sizeof (struct list_node *) * vertices);
  for (from = 0; from < vertices; from++) {
    new_graph->graph[from] = NULL;
  }
  new_graph->vertices = vertices;
  new_graph->edges = g->edges;
  new_graph->time = 0;

  // 전치 연산
  for (from = 0; from < vertices; from++) {
    current_node = g->graph[from];
    while (current_node != NULL) {
      // printf("from : %d, to : %d\n", from, current_node->vertex);
      struct list_node *new_node = (struct list_node *) malloc (sizeof (struct list_node));
      new_node->vertex = from;
      insert_list_node (new_graph->graph, new_node, current_node->vertex);

      current_node = current_node->next; // 빼먹지 말자 하하
    }
  }

  return new_graph;
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
  if (vertexes[0] == NULL) {  // 위쪽에서의 처리가 가정되어 있습니다 - 
    // printf("vertexes가 비어 있어요\n");
    for (i = 0; i < V; i++) {
      struct dfs_node *node = (struct dfs_node *) malloc (sizeof (struct dfs_node));
      node->value = i;
      node->wgb = 'w';
      vertexes[i] = node;
    }
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

// TODO : 왜 함수 2개를 쓰는 것밖에 생각이 안날까..ㅜㅠ 익숙하게 쓸 수 있어지면 좋은 생각이 날 듯. 
// 탐색 종료 순으로 고려하기 위해 tpl_sorted를 참고한다
struct scc_tree *depth_first_search_on_T (struct graph *gT, struct dfs_node **tpl_sorted) {
  struct dfs_node **scc_tree = (struct dfs_node **) malloc (sizeof (struct dfs_node));
  int num_of_scc = 0; //
  int i;

  for (i = 0; i < V; i++) {
    tpl_sorted[i]->wgb = 'w';
  }

  for (i = V - 1; i > -1; i--) { // 그냥 역순으로 돌게 해보았다..
    if (tpl_sorted[i]->wgb == 'w') {
      num_of_scc += 1;
      printf("\tnum_of_scc : %d\n", num_of_scc);
      scc_tree = (struct dfs_node **) realloc (scc_tree, sizeof (struct dfs_node *) * num_of_scc);
      scc_tree[num_of_scc - 1] = NULL;
      // TODO : 왜인지는 정확히 모르겠으나 서로 다른 연결 성분의 시작점마다 이 루프가 돈다
      printf("\t강연결성분의 정점 : %d\n", tpl_sorted[i]->value);
      // dfs_visit_on_T (gT, tpl_sorted, i, scc_tree[num_of_scc - 1]);
      scc_tree[num_of_scc - 1] = dfs_visit_on_T (gT, tpl_sorted, i);
    }
  }

  struct scc_tree *scc = (struct scc_tree *) malloc (sizeof (struct scc_tree));
  scc->tree = scc_tree;
  scc->components = num_of_scc;
  return scc;
}

struct dfs_node *dfs_visit_on_T (struct graph *gT, struct dfs_node **tpl_sorted, int index) {
  printf("\t\tdfs_visit_on_T의 value : %d\n", tpl_sorted[index]->value);
  struct dfs_node *prev_forest = NULL;
  struct dfs_node *node = tpl_sorted[index];
  int i;
  gT->time += 1;
  node->start = gT->time;
  node->wgb = 'g';

  struct list_node *cur = gT->graph[node->value];  // index 정점과 연결된 다른 정점
  while (cur != NULL) {
    int cur_value_index = 0;
    for (i = 0; i < V; i++) {
      // 어쩔수 없다 타잔알고리즘까지 공부해야겠다 시간복잡도가 커져버려 그나마 성질을 이용해서..
      if (tpl_sorted[i]->value == cur->vertex) {
        cur_value_index = i;
      }
    }
    if (tpl_sorted[cur_value_index]->wgb == 'w') {
      tpl_sorted[cur_value_index]->prev = node;
      prev_forest = dfs_visit_on_T (gT, tpl_sorted, cur_value_index); // ☆★☆★
    }
    cur = cur->next;
  }
  node->wgb = 'b';
  gT->time += 1;
  node->end = gT->time;

  // 연결 성분 포레스트
  node->next = prev_forest;
  struct dfs_node *new_forest = node;
  printf("\tforest_root_value %d\n", new_forest->value);
  if (node->next != NULL) {
    printf("\t\t%d\n", node->next->value);
  }
  return new_forest;
}