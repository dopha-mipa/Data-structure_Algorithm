/* 십진법 끝자리 0의 개수 
 * nCm = n!/(n - m)!(m)! 
 * 분자 : n * (n - 1) * ... * (n - m + 1) if n < 2m
 * 분모 : m!
 * 숫자 N까지 배수의 갯수 세기
 */
#include <iostream>
#include <algorithm>

using namespace std;

int cnt_5 (int num);
int cnt_2 (int num);

int main() {
  long n;
  long m;
  cin >> n >> m;

  int two = 0;
  int five = 0;

  two += cnt_2 (n);
  two -= cnt_2 (n - m);
  two -= cnt_2 (m);

  five += cnt_5 (n);
  five -= cnt_5 (n - m);
  five -= cnt_5 (m);
 
  cout << min(two, five) << '\n';

  return 0;
}

int cnt_5 (int num) {
  int cnt = 0;
  for (long i = 5; i <= num; i *= 5) {
    cnt += num / i;
  }
  return cnt;
}

int cnt_2 (int num) {
  int cnt = 0;
  for (long i = 2; i <= num; i *= 2) {
    cnt += num / i;
  }
  return cnt;
}
/*
int cnt_2 (int num) {
  int cnt = 0;
  int max_cnt_pow_2 = floor(log(num) / log(2));

  while (max_cnt_pow_2 > 0) {
    int pow_2 = pow(2, max_cnt_pow_2);

    while (num % pow_2 == 0 && num > 1) {
      cnt += max_cnt_pow_2;
      num /= pow_2;
    }

    max_cnt_pow_2 -= 1;
  }
  return cnt;
}
*/