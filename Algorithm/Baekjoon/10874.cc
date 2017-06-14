#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main () {

  int n;
  vector <int> v;
  string cmp = "1 2 3 4 5 1 2 3 4 5";
  //char s[20];
  string s;

  cin >> n;

  //getline (cin, s);

  for (int i = 0; i < n + 1; i++) {

    getline (cin, s);

    if (s == cmp)
      v.push_back (i);
  }

  vector <int>::iterator iter;

  for (iter = v.begin (); iter != v.end (); iter++)
    cout << *iter << endl;

  return 0;
}