/* 2442 */

#include <iostream>

using namespace std;

int main() {
  int N; cin >> N;

  for (int i = 0; i < N; i++) {
    for (int j = N - i - 1; j > 0; j--) {
      cout << " "; // N + 1 - i
    }
    for (int j = 1; j <= (2 * i) + 1; j++) {
      cout << "*"; // 2N - 1
    }
    cout << endl;
  }
  return 0;
}