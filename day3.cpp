#include <iostream>
#include <stdexcept>


using namespace std;

int weight(char ch) {
  if (ch >= 'a' && ch <= 'z') {
    return (ch - 'a');
  } else if (ch >= 'A' && ch <= 'Z') {
    return (ch - 'A') + 26;
  } else {
    throw runtime_error("Unexpected character: " + string(1, ch));
  }
}


int main(int argc, char* argv[]) {
  long priorities = 0;
  long group_priorities = 0;
  int index = 0;

  int part2[52] = {0};
  for (string line; getline(cin, line);) {
    auto len = line.length();
    int part1[52] = {0};

    if (index == 0) {
      fill(part2, part2 + 52, 0);
    }

    for (int i = 0; i < len / 2; i++) {
      part1[weight(line[i])] += 1;
    }

    for (int j = len / 2; j < len; j++) {
      auto w = weight(line[j]);
      if (part1[w] > 0) {
        priorities += w + 1;
        break;
      }
    }

    for (int k = 0; k < len; k++) {
      auto w = weight(line[k]);
      if (part2[w] == index) {
        part2[w] = index + 1;
        if (index == 2) {
          group_priorities += w + 1;
          break;
        }
      }
    }

    index = (index + 1) % 3;
  }

  cout << priorities << endl;
  cout << group_priorities << endl;

  return 0;
}
