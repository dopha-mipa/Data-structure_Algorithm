/* 2017. 06. 19.
  N-Queen problem with DFS 
  C++ STL
*/
#include <iostream>
#include <cmath>

using namespace std;

void queens(int index, int board[], int N, int depth, int &ans_cnt);
bool promising(int new_Q, int board[]);

int main() {
  int N;
  cout << "Enter the number of Queens : ";
  cin >> N;
  int board[N];
  int ans_cnt = 0;
  cout << "시작 : " << ans_cnt << endl;

  // (0, 0)에 퀸을 두고 시작
  for (int i = 0; i < N; i++) {
    board[0] = i;
    queens(0, board, N, 0, ans_cnt);
  }

  if (ans_cnt == 0) {
    cout << N << " : 정답이 없습니다." << endl;
  } else {
    cout << "정답의 수 : " << ans_cnt << endl;
  }
  return 0;
}

// 이중배열이라면 int (*board)[N] 처럼
void queens(int index, int board[], int N, int depth, int &ans_cnt) {
  int new_Q;

  // 중간 점검용
  // for (int i = 0; i < depth; i++) {
    // cout << "    ";
  // }
  // cout << "queens " << index << " " << board[index] << " "<< depth << endl;

  if (promising(index, board)) {
    if (index == N - 1) {
      /* 출력하는 경우
      cout << "===========Answer==========" << endl;
      for (int i = 0; i < N; i++) {
        cout << board[i] << " ";
      }
      cout << endl;
      */
      ans_cnt += 1;
    }
    else {
      for (new_Q = 0; new_Q < N; new_Q++) {
        board[index + 1] = new_Q;
        queens(index + 1, board, N, depth + 1, ans_cnt);
      }
    }
  } // end if promising
  // return;
} 

bool promising(int new_Q, int board[]) {
  int k = 0;
  bool flag = true;

  while (k < new_Q && flag) {
    if (board[k] == board[new_Q] 
            || abs(board[k] - board[new_Q]) == new_Q - k) {
      flag = false;
      break;
    }
    k++;
  }
  // cout << "flag : " << flag << " new_Q : " << new_Q << endl;
  return flag;
}