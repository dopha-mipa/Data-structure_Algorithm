/*
 * 2017. 02. 07.
 * Introduction to Algorithms 참고 (수정 있음)
 * 드디어 BFS 너비 우선 검색!
 * 초기화 : O(V)
 * 큐에서 삽입, 삭제 : O(1) * V = O(V)
 * 한 간선을 최대 한번 스캔 : O(E)
 * O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define V 6

struct graph {
  struct list_node **graph;
  int vertices;
  int edges;
};

struct list_node { // 리스트 노드
  int vertex; // 꼭지점 이름
  struct list_node *next;
};

struct queue {
  struct queue_node *front;
  struct queue_node *rear;
  int length;
};

struct queue_node {
  int value;
  char wgb; // white, grey. black 중 하나의 값을 가짐
  int distance; // 시작점으로부터의 거리
  struct queue_node *prev; // 직전 꼭지점 - 이게 왜 prev인가 했더니... 아구구구 이유가 있는 거였어 
  struct queue_node *next; // 편의상 추가
};

struct graph *matrix_to_list (int **matrix, int vertices);
void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex);

struct queue *breadth_first_search (struct graph *g, int start);
void bfs_queueing (struct graph *g, struct queue * que, struct queue_node **vertexes, struct queue *result);
void enqueue (struct queue *q, struct queue_node *node);
struct queue_node *dequeue (struct queue *q);
struct queue_node *traverse_queue_node (struct queue *q, int value);

int main() {
  // 먼저 리스트를 준비해 줍시다. 
  int adj_mat[6][6] = {{0, 1, 0, 1, 0, 0},
                       {0, 0, 0, 0, 1, 0},
                       {0, 0, 0, 0, 1, 1},
                       {0, 1, 0, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0},
                       {0, 0, 0, 0, 0, 1}};
  int i, j;
  int **matrix = (int **) malloc (sizeof (int *) * V);
  for (i = 0; i < V; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * 6);
    memcpy (matrix[i], adj_mat[i], sizeof (int) * 6);
  }
  // 리스트로 표현된 그래프 반환
  struct graph *g = matrix_to_list (matrix, V);

  // BFS
  int start = 2;
  struct queue *result = breadth_first_search (g, start);
  struct queue_node *head = result->front;

  // 결과 출력
  while (head != NULL) {
    printf("\tval : %d\twgb: %c\tdistance : %d\n", head->value, head->wgb, head->distance);
    head = head->next;
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

  return g;
}

void insert_list_node (struct list_node **graph, struct list_node *new_node, int vertex) {
  if (graph[vertex] != NULL) {
    new_node->next = graph[vertex]; // graph[vertex] = graph[vertex][0] 이라고 생각해서!
  }
  graph[vertex] = new_node;
}

// BFS에 관련된 함수들
struct queue *breadth_first_search (struct graph *g, int start) {
  struct queue_node *vertexes[V];
  int i;

  for (i = V; i > 0; i--) {
    struct queue_node *q = (struct queue_node *) malloc (sizeof (struct queue_node));
    q->value = i - 1;
    q->wgb = 'w';
    q->distance = 999;
    q->prev = NULL;
    q->next = NULL;
    vertexes[i - 1] = q;
  }

  struct queue_node *s = vertexes[start]; // 사용자가 지정한 꼭지점
  s->wgb = 'g';
  s->distance = 0;
  s->next = NULL;

  struct queue *que = (struct queue *) malloc (sizeof (struct queue));
  que->front = NULL;
  que->rear = NULL;
  // que->length = 0;
  enqueue (que, s);

  struct queue *result = (struct queue *) malloc (sizeof (struct queue));
  result->front = NULL;
  result->rear = NULL;

  bfs_queueing (g, que, vertexes, result);

  // ☆ TODO 방향 그래프에서ㅜㅠ outer degree는 있지만 inner degree가 없는 정점들을 위해
  for (i = 0; i < V; i++) {
    if (vertexes[i]->wgb == 'w') {
      struct queue_node *ss = vertexes[i];
      ss->wgb = 'g';
      // 거리는 초기화 안함
      ss->next = NULL;
      enqueue (que, ss);
      bfs_queueing (g, que, vertexes, result);
    }
  }

  return result;
}

void bfs_queueing (struct graph *g, struct queue * que, struct queue_node **vertexes, struct queue *result) {
  while (que->front != NULL) {
    struct queue_node *u = dequeue (que);
    enqueue (result, u);

    /* 
     * 인접 리스트 표현을 통해 간선이 있는 정점을 찾는다.
     * 간선 끝의 정점이 흰색이라면
     *    회색 / 거리 / prev + 내 경우에는 next까지
     */
    struct list_node *cur = g->graph[u->value];
    while (cur != NULL) {
      int vertex = cur->vertex;
      struct queue_node *node = vertexes[vertex];

      if (node->wgb == 'w') {
        node->wgb = 'g';
        node->distance = u->distance + 1;
        node->prev = u;
        enqueue (que, vertexes[vertex]);
      }
      cur = cur->next;
    }
    u->wgb = 'b';
  }
}

void enqueue (struct queue *q, struct queue_node *node) { // 큐의 끝에만 삽입
  printf("\tenqueue할 값 : %d\n", node->value);
  node->next = NULL;
  if (q->rear != NULL) {
    q->rear->next = node;
  }
  if (q->front == NULL) {
    q->front = node;
  }
  q->rear = node;
  // q.length += 1;
}

struct queue_node *dequeue (struct queue *q) {
  struct queue_node *head = q->front;
  if (q->front->next != NULL) { // 내 생각에는 NULL이면 그냥 NULL 될 것 같았는데...
    q->front = q->front->next;
  } else {
    q->front = NULL;
    q->rear = NULL;
    // head->next = NULL;
  }
  printf("\tdequeue value : %d\n", head->value);
  return head;
}

struct queue_node *traverse_queue_node (struct queue *q, int value) {
  struct queue_node *current = q->rear;

  while (current != NULL && current->value == value) {
    current = current->next;
  }
  return current;
}