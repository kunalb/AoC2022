#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;

tuple<tuple<int, int>, tuple<int, int>> parseLine(const string &line) {
  size_t pos = line.find_first_of(",");
  size_t first_split = line.find_first_of("-");
  size_t second_split = line.find_first_of("-", pos);

  auto line_str = line.c_str();
  return make_tuple(
      make_tuple(atoi(line_str), atoi(line_str + first_split + 1)),
      make_tuple(atoi(line_str + pos + 1), atoi(line_str + second_split + 1)));
}

int main(int argc, char *argv[]) {

  size_t count_full = 0;
  size_t count_partial = 0;

  for (string line; getline(cin, line);) {
    auto points = parseLine(line);
    auto point1 = get<0>(points);
    auto point2 = get<1>(points);

    if ((get<0>(point1) <= get<0>(point2) && get<1>(point1) >= get<1>(point2)) ||
        (get<0>(point2) <= get<0>(point1) && get<1>(point2) >= get<1>(point1))) {
      count_full++;
    }

    if ((get<0>(point2) <= get<0>(point1) && get<0>(point1) <= get<1>(point2)) ||
        (get<0>(point1) <= get<0>(point2) && get<0>(point2) <= get<1>(point1))) {
      count_partial++;
    }
  }

  cout << count_full << endl;
  cout << count_partial << endl;

  return 0;
}
