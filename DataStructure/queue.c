/*
 * Introduction to Algorithms 
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct queue {
  struct queue_node *front;
  struct queue_node *rear;
}

int is_empty ()

struct queue_node {
  int value;
  struct queue_node *prev;
  struct queue_node *next;
};

int main() {


  return 0;
}

struct queue_node *search (struct queue *q, int value) {
  struct queue_node *current = q->front;

  while (current != NULL && current->value == value) {
    current = current->next;
  }
  return current;
}

void insert (struct queue *q, struct queue_node *node) {
  node->next = q->front;
  if (q->front != NULL) {
    q->front->prev = node;
  }
  q->front = node;
  node->prev = NULL;
}

void delete (struct queue *q, struct queue_node *node) {
  if (node->prev != NULL) {
    node->prev->next = node->next;
  } else {
    q->front = node->next;
  }
  if (node->next != NULL) {
    node->next->prev = node->prev;
  } else {
    g->rear = node->prev;
  }
}