#include <iostream>
#include <numeric>

using namespace std;


bool uniq(const char * lastn, size_t n) {
  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (lastn[i] == lastn[j])
        return false;
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  char ch;

  int chars_read = 0;

  char last4[4] = { 0 };
  char last14[14] = { 0 };

  size_t ptr4 = 0;
  size_t ptr14 = 0;

  bool solved4 = false;
  bool solved14 = false;

  while (cin.get(ch)) {
    chars_read += 1;

    last4[ptr4] = ch;
    ptr4 = (ptr4 + 1) % 4;

    last14[ptr14] = ch;
    ptr14 = (ptr14 + 1) % 14;

    if (uniq(last4, 4) && chars_read >= 4 && !solved4) {
      cout << " 4 " << chars_read << endl;
      solved4 = true;
    }

    if (uniq(last14, 14) && chars_read >= 14 && !solved14) {
      cout << "14 " << chars_read << endl;
      solved14 = true;
    }
  }

  return 0;
}
