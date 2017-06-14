/* 9012 - Parenthesis String*/
#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main() {
  int N;
  cin >> N;

  while (N--) {
    string ps;
    cin >> ps;

    stack<int> s;

    int flag = 1;
    for (int i = 0; i < ps.length(); i++) {
      if (ps[i] == '(') {
        s.push(ps[i]);
      } else if (ps[i] == ')') {
        if (s.empty()) {
          cout << "NO" << "\n";
          flag = 0;
          break;
        } else {
          s.pop();
        }
      }
    }
    if (flag == 1 && s.empty()) {
      cout << "YES" << "\n";
    } else if (!s.empty()) {
      cout << "NO" << "\n";
    }

  }
  return 0;
}