#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <variant>

using namespace std;

using NestedList = std::variant<int, unique_ptr<struct S>>;

struct S {
  list<NestedList> lst;

  S() = default;
  ~S() = default;
};

ostream &operator<<(ostream &os, const NestedList &nl) {
  if (const int *val = get_if<int>(&nl)) {
    os << *val;
  } else {
    const auto s = &get<1>(nl);
    os << "[";
    for (const auto &x : (*s)->lst) {
      os << x;
      os << ", ";
    }
    os << "]";
  }

  return os;
}

NestedList readList(const string &s) {
  deque<NestedList> state;
  bool justOpened = false;

  for (auto ch : s) {
    if (ch == '[') {
      NestedList l = make_unique<S>();
      state.push_back(move(l));
    } else if (ch == ']' || ch == ',') {
      // cout << "At " << ch << endl;
      // for (auto &nl : state) {
      //   cout << "  " << nl << endl;
      // }
      if (ch == ']' && justOpened)
        continue;

      auto res = move(state.back());
      state.pop_back();
      get<1>(state.back())->lst.push_back(move(res));

      // cout << "After " << ch << endl;
      // for (auto &nl : state) {
      //   cout << "  " << nl << endl;
      // }
    } else if (ch >= '0' && ch <= '9') {
      if (int *val = get_if<int>(&state.back())) {
        *val = (*val) * 10 + (ch - '0');
      } else {
        state.push_back(ch - '0');
      }
    } else {
      throw invalid_argument("Can't handle: `" + std::string(1, ch) + "`");
    }

    justOpened = ch == '[';
  }

  return move(state.back());
}

list<NestedList> readLists() {
  list<NestedList> result;

  string line;
  while (!cin.eof()) {
    getline(cin, line);
    result.push_back(readList(line));

    getline(cin, line);
    result.push_back(readList(line));

    getline(cin, line);
  }

  return result;
}

int main() {
  // auto input = "[]";
  // cout << readList(input) << endl;
  auto lists = readLists();
}
