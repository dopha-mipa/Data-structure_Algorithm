#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

int* hill_climbing(int N);
int *random_queens(int board[], int N);
int attack_score(int board[], int N);
int *next_successor(int board[], int N, int &min_score);

int main() {
  int N;
  cout << "Enter the number of Queens : ";
  cin >> N;

  int* board = hill_climbing(N);
  if (board == NULL) {
    cout << "답을 찾지 못했어요.." << endl;
  }
  else {
    cout << *board << endl;
  }

  return 0;
}

int* hill_climbing(int N) {
  int *min_board = new int[N];
  int *cur_board = new int[N];;
  int MAX_LOCAL_SEARCH = 2 * N;

  // initialize
  min_board = random_queens(min_board, N);
  int min_score = attack_score(min_board, N); 

  memcpy(cur_board, min_board, N * sizeof(int));
  cur_board = next_successor(cur_board, N, min_score);

  int local_cnt = 0;
  int total_cnt = 0;
  while (min_score != 0 && total_cnt < N * pow(2, N)) {
    int score = attack_score(cur_board, N);

    if (score < min_score) {
      if (score == 0) {
        for (int i = 0; i < N; i++) {
          cout << cur_board[i] << " ";
        }
        cout << endl;
        min_score = score;
      }
      else {
        memcpy(min_board, cur_board, N * sizeof(int));
        min_score = score;

        cur_board = next_successor(cur_board, N, min_score);
      }
    }

    else {
      local_cnt += 1;
      if (local_cnt >= MAX_LOCAL_SEARCH) {
        cur_board = random_queens(cur_board, N);
        local_cnt = 0;
        // min_score = attack_score(cur_board, N);
      }
      else {
        cur_board = next_successor(cur_board, N, min_score);
      }
    }
    
    total_cnt += 1;
  }

  if (total_cnt > N * pow(2, N)) {
    return NULL;
  }
  return min_board;
}

int *random_queens(int board[], int N) {
  srand(time(0));
  int new_Q;
  for (int i = 0; i < N; i++) {
    new_Q = rand() % N;
    if (board[i] == new_Q && i < N / 2) {
      new_Q = rand() % N;
    }
    board[i] = new_Q;
  }

  return board;
} 

int attack_score(int board[], int N) {
  int can_attack = 0;

  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      if (board[i] == board[j] || abs(board[i] - board[j]) == j - i) {
        can_attack += 1;
      }
    }
  }

  return can_attack;
}

int *next_successor(int board[], int N, int &min_score) {
  int *temp_board = new int[N];
  memcpy(temp_board, board, N * sizeof(int));

  int new_Q[2] = {-10, -10};
  int cur_min_score = min_score;

  int row = 0;
  while (row < N) {
    int origin_Q = board[row];

    for (int j = 0; j < N; j++) {
      temp_board[row] = j;
      int score = attack_score(temp_board, N);

      if (score < cur_min_score) {
        new_Q[0] = row;
        new_Q[1] = j;
        cur_min_score = score;
      }
    }

    if (cur_min_score == 0) {
      break;
    }
    temp_board[row] = origin_Q;
    row += 1;
  }

  if (new_Q[0] < N && new_Q[1] < N && new_Q[0] > -1 && new_Q[1] > -1){
    board[new_Q[0]] = new_Q[1];
  }
  else if (new_Q[0] == -10 && new_Q[1] == -10 ) {
    board = random_queens(board, N);
    min_score = attack_score(board, N);
  }
  delete[] temp_board;


  return board;
}