/* 2017. 06. 19.
  N-Queen problem with BFS  (with pruning)
  C++ STL
  C++에서 Tree에는 map을 사용한다는걸 처음 배우다
*/
#include <iostream>
#include <cmath>
#include <queue>
#include <vector>

using namespace std;

struct Node {
  vector<int> board;
  int depth;
};

bool promising(Node node);

int main() {
  int N;
  cout << "Enter the number of Queens : ";
  cin >> N;

  int board[N];
  int ans_cnt = 0;

  queue<Node> que;
  for (int i = 0; i < N; i++){
    vector<int> b;
    b.push_back(i);
    Node ri = {b, 0};
    que.push(ri);
  }

  while (!que.empty()) {
    Node node = que.front();
    que.pop();

    if (promising(node)) {
      if (node.board.size() == N) {
        for (int j = 0; j < N; j++) {
          cout << node.board[j] << ' ';
        }
        cout << endl;
        ans_cnt += 1;
      }
      else { // 다음 탐색
        for (int new_Q = 0; new_Q < N; new_Q++) {
          vector<int> new_board = node.board;
          new_board.push_back(new_Q);
          Node new_node = {new_board, node.depth + 1};
          que.push(new_node);
        }
      }
    } // end if promising
  } // end while

  if (ans_cnt == 0) {
    cout << N << " : 정답이 없습니다." << endl;
  } else {
    cout << "정답의 수 : " << ans_cnt << endl;
  }

  return 0;
}

bool promising(Node node) {
  int k = 0;
  bool flag = true;
  int depth = node.depth;

  while (k < node.depth && flag) {
    if (node.board[k] == node.board[depth] 
            || abs(node.board[k] - node.board[depth]) == depth - k) {
      flag = false;
      break;
    }
    k++;
  }

  return flag;
}