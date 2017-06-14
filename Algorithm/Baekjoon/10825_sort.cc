/* 10825 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Student {
  string name;
  int korean;
  int english;
  int math;
};

bool cmp(const Student &s1, const Student &s2);

int main() {
  int N; cin >> N;
  vector<Student> v(N);

  for (int i = 0; i < N; i++) {
    cin >> v[i].name >> v[i].korean >> v[i].english >> v[i].math;
  }
  stable_sort(v.begin(), v.end(), cmp);

  for (int i = 0; i < N; i++) {
    cout << v[i].name << '\n';
  }
  return 0;
}

bool cmp(const Student &s1, const Student &s2) {
  if (s1.korean > s2.korean) {
    return true;
  }
  else if (s1.korean == s2.korean) {
    if (s1.english < s2.english) {
      return true;
    }
    else if (s1.english == s2.english) {
      if (s1.math > s2.math) {
        return true;
      }
      else if (s1.math == s2.math) {
        return s1.name < s2.name;
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

/*
12
Junkyu 50 60 100
Sangkeun 80 60 50
Sunyoung 80 70 100
Soong 50 60 90
Haebin 50 60 100
Kangsoo 60 80 100
Donghyuk 80 60 100
Sei 70 70 70
Wonseob 70 70 90
Sanghyun 70 70 80
nsj 80 80 80
Taewhan 50 60 90
*/