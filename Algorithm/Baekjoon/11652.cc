/* 11652 */
#include <cstdio>
#include <algorithm>

using namespace std;

int main() {
    int N;
    scanf("%d", &N);
    long long arr[1000001] = {0, };

    for (int i = 0; i < N; i++) {
        scanf("%lld", arr + i);
    }

    sort(arr, arr + N);

    long long answer = 0;
    int max_count = 0;
    int temp_count = 0;
    for (int i = 0; i < N; i++) {
        temp_count += 1;
        if (arr[i] != arr[i + 1]) {
            if (max_count < temp_count) {
                answer = arr[i];
                max_count = temp_count;
            }
            temp_count = 0;
        }
    }

    printf("%lld\n", answer);

    return 0;
}

/*
5
1
2
1
2
1
*/
/*
6
1
2
1
2
1
2
*/
