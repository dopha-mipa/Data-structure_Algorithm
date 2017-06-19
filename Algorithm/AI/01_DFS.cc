/* 2017. 06. 19.
  N-Queen problem with DFS 
  C++ STL
*/
#include <iostream>
#include <cmath>

using namespace std;

void queens(int depth, int board[], int N, int &ans_cnt);
bool promising(int new_Q, int board[]);

int main() {
  int N;
  cout << "Enter the number of Queens : ";
  cin >> N;

  int board[N];
  int ans_cnt = 0;

  for (int i = 0; i < N; i++) {
    board[0] = i;
    queens(0, board, N, ans_cnt);
  }

  if (ans_cnt == 0) {
    cout << N << " : 정답이 없습니다." << endl;
  } else {
    cout << "정답의 수 : " << ans_cnt << endl;
  }

  return 0;
}

void queens(int depth, int board[], int N, int &ans_cnt) {
  if (promising(depth, board)) {
    if (depth == N - 1) {
      for (int i = 0; i < N; i++) {
        cout << board[i] << "  ";
      }
      cout << endl;
      ans_cnt += 1;
    }
    else {
      for (int new_Q = 0; new_Q < N; new_Q++) {
        board[depth + 1] = new_Q;
        queens(depth + 1, board, N, ans_cnt);
      }
    }
  } // end if promising
} 

bool promising(int depth, int board[]) {
  int k = 0;
  bool flag = true;

  while (k < depth && flag) {
    if (board[k] == board[depth] 
            || abs(board[k] - board[depth]) == depth - k) {
      flag = false;
      break;
    }
    k++;
  }

  return flag;
}