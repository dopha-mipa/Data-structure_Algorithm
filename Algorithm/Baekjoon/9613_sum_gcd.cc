/* 9613  :  solution of BaekJoon */
// 뭔가 C++로만 쓰면 더 예쁠 것 같긴 해..
#include <cstdio>

int gcd(int x, int y) {
  if (y == 0) {
    return x;
  } else {
    return gcd(y, x % y);
  }
}

int main() {
  int N;
  scanf("%d", &N);

  while (N--) {
    int a[101];
    int len;
    scanf("%d", &len);

    for (int i = 0; i < len; i++) {
      scanf("%d", &a[i]);
    }

    int sum = 0;
    for (int i = 0; i < len; i++) {
      for (int j = i + 1; j < len; j++) {
        sum += gcd(a[i], a[j]);
      }
    }

    printf("%d\n", sum);
  }
  return 0;
}
/*
// 삽질의 흔적.. sstream이라는게 존재한다는 것만 알았다.
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
  std::istringstream iss;
  string input;
  string val;
  vector<int> v;
  int N;
  cin >> N;
  for (int n = 0; n < N; n++) {
    getline(cin, input);
    iss.str(input);
    while (!iss.eof()) {
      iss >> val;
      cout << val;
    }
  }
  return 0;
}
std::vector<int> sum_of_gcd(vector<int> v) {
  int summation = 0;
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v.size(); j++) {

    }
  }
}
*/

/*
3
4 10 20 30 40
3 7 5 12
3 125 15 25
*/