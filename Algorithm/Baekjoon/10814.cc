/* 10814 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Person {
  int age;
  string name;
};

bool compare_as_ints(const Person &i, const Person &j) {
  return (i.age < j.age);
}

int main() {
  int N; cin >> N;
  vector<Person> v(N);

  for (int i = 0; i < N; i++) {
    cin >> v[i].age >> v[i].name;
  }
  stable_sort(v.begin(), v.end(), compare_as_ints);

  for (int i = 0; i < N; i++) {
    cout << v[i].age << ' ' << v[i].name << '\n';
  }

  return 0;
}

/*
3
21 Junkyu
21 Dohyun
20 Sunyoung
*/
