#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_stacks(const vector<char> *stacks) {
  for (int i = 0; i < 10; i++) {
    for (char ch : stacks[i])
      cout << ch << " ";
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  vector<char> stacks[10], stacks2[10];
  bool instructions = false;

  for (string line; getline(cin, line);) {
    if (line == "") {
      instructions = true;
      for (int i = 0; i < 10; i++) {
        stacks2[i] = stacks[i];
      }
      continue;
    }

    if (!instructions) {
      for (int i = 0; i < 10; i++) {
        char ch = line[1 + i * 3 + i];
        if (ch != ' ') {
          stacks[i].insert(stacks[i].begin(), ch);
        }
      }
    } else {
      int count = atoi(line.c_str() + 5);
      int start = atoi(line.c_str() + line.find_last_of('m') + 2) - 1;
      int end = atoi(line.c_str() + line.find_last_of('o') + 2) - 1;

      for (int i = 0; i < count; i++) {
        stacks[end].push_back(stacks[start].back());
        stacks[start].pop_back();

        stacks2[end].insert(stacks2[end].end() - i, stacks2[start].back());
        stacks2[start].pop_back();
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    cout << stacks[i].back();
  }
  cout << endl;

  for (int i = 0; i < 10; i++) {
    cout << stacks2[i].back();
  }
  cout << endl;

  return 0;
}
