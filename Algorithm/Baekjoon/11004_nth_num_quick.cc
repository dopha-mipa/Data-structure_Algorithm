/**
 * 2017. 01. 17. 나윤
 */
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 수 N개가 주어진다. 
 * N개의 수를 오름차순 정렬했을 때, 앞에서부터 K번째에 있는 수를 구하는 프로그램을 작성하시오.
*/
int main() {
  int n, k;
  int i;
  scanf("%d %d", &n, &k);
  int a[n];

  k -= 1;
  for (i = 0; i < n; i++) {
    scanf("%d", &a[i]);
  }
  // C로 푼다면 quick을 이용해서 풀 수 있다.
  nth_element(a, a + k, a + n);  // n번째 큰 숫자를 기준으로 나눠주는 알고리즘
  printf("%d\n", a[k]);
  return 0;
}