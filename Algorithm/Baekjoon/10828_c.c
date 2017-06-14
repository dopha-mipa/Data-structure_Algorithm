/* 10828 - stack with c */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define STK_SIZE 10000

struct stack {
  int arr[STK_SIZE];
  int top;
};

struct stack *init_stk ();
int push (struct stack *s, int num);
int pop (struct stack *s);
int size (struct stack *s);
int empty (struct stack *s);
int top(struct stack *s);

int main() {
  struct stack *stk = init_stk ();;
  char cmd[5];

  int N;
  scanf("%d", &N);

  int num;
  while (N > 0) {
    scanf("%s", cmd);

    if (strncmp(cmd, "push", 4) == 0) {
      scanf("%d", &num);
      push (stk, num);
      N -= 1;
      continue;

    } else if (strncmp(cmd, "pop", 3) == 0) {
      printf("%d\n", pop(stk));
      N -= 1;
      continue;

    } else if (strncmp(cmd, "top", 3) == 0) {
      printf("%d\n", top(stk));
      N -= 1;
      continue;

    } else if ((strncmp(cmd, "empty", 5) == 0)) {
      printf("%d\n", empty(stk));
      N -= 1;
      continue;

    } else if ((strncmp(cmd, "size", 4) == 0)) {
      printf("%d\n", size(stk));
      N -= 1;
      continue;
    }
  }

  return 0;
}

struct stack *init_stk () {
  struct stack *s = (struct stack *) malloc (sizeof (struct stack));
  s->top = 0;
  return s;
}

int push (struct stack *s, int num) {
  if (s->top >= STK_SIZE) {
    return -1;
  }
  s->arr[s->top] = num;
  s->top += 1;
  return 0;
}

int pop (struct stack *s) {
  if (s->top < 1) {
    return -1;
  }
  s->top -= 1;
  int popped = s->arr[s->top];
  s->arr[s->top] = 0;
  return popped;
}

int size (struct stack *s) {
  return s->top;
}

int empty (struct stack *s) {
  if (s->top == 0) {
    return 1;
  }
  return 0;
}

int top(struct stack *s) {
  if (s->top == 0 || s->top >= STK_SIZE) {
    return -1;
  }
  return s->arr[s->top - 1];
}
