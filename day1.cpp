#include <array>
#include <iostream>
#include <numeric>
#include <string>

using namespace std;


int main(int argc, char* argv[]) {
  long max_elf = 0;
  array<long, 3> max_elves{ {0, 0, 0} };
  long cur_elf = 0;

  for (string line; getline(cin, line);) {
    if (line == "") {
      max_elf = max(max_elf, cur_elf);

      for (int i = 0; i < 3; i++) {
        if (max_elves[i] < cur_elf) {
          for (int j = i + 1; j < 3; j++) {
            max_elves[j] = max_elves[j - 1];
          }
          max_elves[i] = cur_elf;
          break;
        }
      }

      cur_elf = 0;
    } else {
      cur_elf += stol(line);
    }
  }

  cout << max_elf << endl;
  cout << accumulate(max_elves.begin(), max_elves.end(), 0) << endl;
}
