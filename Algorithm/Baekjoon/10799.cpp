/* 10799 쇠막대기 - 9012번과 연계해서 */
#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main() {
  string ps;
  cin >> ps;
  stack<int> s;

  int i = 0;
  int sum = 0;
  int len = ps.length(); // for debugging
  while (i < ps.length()) {
    if (ps[i] == '(') {
      if (i + 1 < ps.length() && ps[i + 1] == ')') { // 레이저~!
        sum += s.size();
        // cout << "cutted i :" << i << " sum += " << s.size() << endl;
        i += 2;
      } else {
        s.push(ps[i]);
        // cout << "pushed i : " << i << endl;
        i += 1;
      }
    } else if (ps[i] == ')') {
      sum  += 1;
      s.pop();
      // cout << "end one i : " << i << endl;
      i += 1;
    }
  } // end for

  cout << sum << "\n";

  return 0;
}