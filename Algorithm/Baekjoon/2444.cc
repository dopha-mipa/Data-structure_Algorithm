/* 2444 */

#include <iostream>

using namespace std;

int main() {
  int N;
  cin >> N;
  int i, j;
  
  for (int n = 0; n < N - 1; n++) {
    for (i = 0; i < N - n - 1; i++) {
      cout << " ";
    }

    for (j = i; j < N + n; j++) {
      cout << "*";
    }
    cout << endl;
  }

  for (int i = 0; i < (2 * N) - 1; i++) {
    cout << "*";
  }
  cout << endl;

  for (int n = 0; n < N - 1; n++) {
    for (i = 0; i < n + 1; i++) {
      cout << " ";
    }

    for (j = i; j < (2 * N) - i - 1; j++) {
      cout << "*";
    }
    cout << endl;
  }
  return 0;
}


/*
input
5
*/

/*
output
    *
   ***
  *****
 *******
*********
 *******
  *****
   ***
    *
   */