/*
 * 2017. 03. 28. minimum spanning tree - prim
 * 프림에서는 사이클이 생기지 않습니다.
 * 
 * 2017. 04. 04. 새로 시작 ㅜㅠ
 * min heap 우선 순위 큐를 이용한 Prim Algorithm이 있지만
 * 수업시간에 쓴 별도의 pop이 필요하지 않은 방식으로.. (-1 표기)
 * Q : 그런데 이 방식은 행렬이랑 더 잘 맞나? 아닐텐데
 * 수정해서 2017. 04. 05. 완료 
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define V 9

// for graph - e_node
struct graph {
  struct e_node **graph;
  int v;
  int e;
};

struct e_node {
  int from; // starting pt
  int to; // end pt
  int weight;
  int add_prim; // prim 간선인지의 여부
  struct e_node *next;
};

struct e_node *prim_search (int **matrix, int v, int start);
struct graph *matrix_to_list (int **matrix, int vertices);
void insert_list_node (struct e_node **graph, struct e_node *new_node, 
                       int vertex);

int main() {
  int graph_matrix[V][V] = {{0, 45, 220, 79, 0, 0, 0, 0, 0}, // A
                            {45, 0, 0, 168, 98, 0, 0, 0, 0}, // B
                            {220, 0, 0, 0, 0, 10, 0, 0, 19}, // C
                            {79, 168, 0, 0, 15, 33, 60, 0, 0}, // D
                            {0, 98, 0, 15, 0, 0, 133, 63, 0}, // E
                            {0, 0, 10, 33, 0, 0, 93, 0, 65}, // F
                            {0, 0, 0, 60, 133, 93, 0, 100, 111}, // G
                            {0, 0, 0, 0, 63, 0, 100, 0, 0}, // H
                            {0, 0, 19, 0, 0, 65, 111, 0, 0}}; // I
  int i;

  int **matrix = (int **) malloc (sizeof (int *) * V);
  for (i = 0; i < V; i++) {
    matrix[i] = (int *) malloc (sizeof (int) * V);
    memcpy (matrix[i], graph_matrix[i], sizeof (int) * V);
  }
  // 인접 리스트로 표현된 그래프 
  // struct graph *g = matrix_to_list (matrix, V);

  int start = 0;
  struct e_node *prim_edges = prim_search (matrix, V, start);
  
  for (i = 0; i < V - 1; i++) {
    printf ("from : %d to : %d weight : %d\n", prim_edges[i].from, prim_edges[i].to, prim_edges[i].weight);
  }


  return 0;
}
/*
 * matrix : 인접리스트로 이걸 풀면 더 뺑이 돌 것 같아서.. 행렬
 * v : 정점의 수
 * start : 시작점
 */
struct e_node *prim_search (int **matrix, int v, int start) {
  // 가장 가까운 노선의 이전 정점 - near와 dist는 한세트
  int *near_pre_v = (int *) malloc (sizeof (int) * v);
  // 프림 정점으로부터의 거리
  int *dist_prim = (int *) malloc (sizeof (int) * v);
  memset (dist_prim, 0, sizeof(int) * v);

  struct e_node *prim_edges = (struct e_node *) malloc (sizeof (struct e_node) * (v - 1));
  int i, j;

  for (i = 0; i < v; i++) {
    near_pre_v[i] = start;
    dist_prim[i] = matrix[start][i];
    if (matrix[start][i] == 0) {
      dist_prim[i] = 9999;
    }
  }
  dist_prim[start] = -1;

  int min_dist;
  int vnear = start; // 프림에 추가될 정점
  for (i = 0; i < v - 1; i++) { // 추가할 간선이 최종 n - 1 개 니까
    min_dist = 9999;

    for (j = 0; j < v; j++) {
      if (j != start && dist_prim[j] < min_dist && dist_prim[j] > 0) {
        min_dist = dist_prim[j];
        vnear = j;
      }
    }

    prim_edges[i].from = near_pre_v[vnear];
    prim_edges[i].to = vnear;
    prim_edges[i].weight = min_dist;
    dist_prim[vnear] = -1; // 프림에 추가됨

    for (j = 0; j < v; j++) {
      if (matrix[j][vnear] != 0 && matrix[j][vnear] < dist_prim[j] && j != start) {
        near_pre_v[j] = vnear;
        dist_prim[j] = matrix[j][vnear];
      }
    }
  }

  return prim_edges;
}

// 가중치가 있는 그래프를 위해 변경한 코드
struct graph *matrix_to_list (int **matrix, int vertices) {
  struct e_node **adj_list = (struct e_node **) malloc (sizeof (struct e_node *) * vertices);
  int edges = 0;
  int from, to;

  for (from = 0; from < vertices; from++) {
    for (to = 0; to < vertices; to++) {
      if (matrix[from][to] > 0) { // 가중치가 있다면
        struct e_node *new_node = (struct e_node *) malloc (sizeof (struct e_node));
        new_node->from = from;
        new_node->to = to;
        new_node->weight = matrix[from][to]; // 가중치를 함께 기록
        new_node->add_prim = 0; // 프림 간선이 아니다
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

void insert_list_node (struct e_node **graph, struct e_node *new_node, 
                       int vertex) {
  if (graph[vertex] != NULL) {
    new_node->next = graph[vertex];
  }
  graph[vertex] = new_node;
}

/*
* 나윤아 안녕 이건 키감이 좋구나
* 자원이랑 다니느라 많이 힘들지
* 힘내
* - 2017년 3월 31일 원민언니 ㅋㅋㅋㅋ
*/
