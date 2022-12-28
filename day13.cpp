#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <variant>
#include <vector>

using namespace std;

using NestedList = std::variant<int, shared_ptr<struct S>>;

struct S {
  vector<NestedList> lst;

  S() = default;
  ~S() = default;
};

ostream &operator<<(ostream &os, const NestedList &nl) {
  if (const int *val = get_if<int>(&nl)) {
    os << *val;
  } else {
    const auto s = &get<1>(nl);
    os << "[";

    bool first = true;
    for (const auto &x : (*s)->lst) {
      if (!first)
        os << ",";
      os << x;
      first = false;
    }
    os << "]";
  }

  return os;
}

NestedList asList(NestedList x) {
  if (holds_alternative<int>(x)) {
    auto s = make_unique<S>();
    s->lst.push_back(move(x));
    return s;
  }

  return x;
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
      if (ch == ']' && justOpened) {
        justOpened = false;
        continue;
      }

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

  stringstream stream;
  stream << state.back();
  if (s != stream.str())
    throw runtime_error("Failed matching!\nOriginal: " + s +
                        "\nParsed: " + stream.str());

  return move(state.back());
}

optional<bool> compare(const NestedList &left, const NestedList &right) {
  // cout << "  Comparing " << left << " and " << right << endl;
  if (const int *left_int = get_if<int>(&left),
      *right_int = get_if<int>(&right);
      left_int && right_int) {
    if (*left_int < *right_int)
      return true;
    else if (*left_int > *right_int)
      return false;
    else
      return {};
  }

  if (const auto *left_s = get_if<1>(&left), *right_s = get_if<1>(&right);
      left_s && right_s) {
    size_t i = 0;
    size_t left_size = (*left_s)->lst.size();
    size_t right_size = (*right_s)->lst.size();

    while (i < left_size && i < right_size) {
      auto res = compare((*left_s)->lst[i], (*right_s)->lst[i]);
      if (res.has_value())
        return res;
      i++;
    }

    if (left_size != right_size)
      return left_size < right_size;

    return {};
  }

  NestedList left_val = left;
  NestedList right_val = right;
  size_t left_size = 1;
  size_t right_size = 1;

  if (const auto *left_s = get_if<1>(&left)) {
    left_size = (*left_s)->lst.size();
    if (left_size > 0)
      left_val = (*left_s)->lst[0];
  }

  if (const auto *right_s = get_if<1>(&right)) {
    right_size = (*right_s)->lst.size();
    if (right_size > 0)
      right_val = (*right_s)->lst[0];
  }

  if (left_size != 0 && right_size != 0) {
    auto res = compare(left_val, right_val);
    if (res.has_value())
      return res;
  }

  if (left_size == right_size) {
    return {};
  }

  return left_size < right_size;
}

list<tuple<NestedList, NestedList>> readLists() {
  list<tuple<NestedList, NestedList>> result;

  string line;
  while (!cin.eof()) {
    getline(cin, line);
    auto left = readList(line);

    getline(cin, line);
    auto right = readList(line);

    getline(cin, line);
    result.emplace_back(move(left), move(right));
  }

  return result;
}

int main() {
  auto lists = readLists();
  vector<NestedList> flatList;
  for (auto &pair : lists) {
    flatList.push_back(get<0>(pair));
    flatList.push_back(get<1>(pair));
  }

  ulong sum = 0;
  ulong i = 1;

  for (auto &pair : lists) {
    // cout << i << ") " << "Comparing " << endl
    //      << "  " << get<0>(pair) << endl
    //      << "  " << get<1>(pair) << endl;
    if (compare(get<0>(pair), get<1>(pair)).value()) {
      sum += i;
      // cout << i << " Correct" << endl;
    } else {
      // cout << i << " Incorrect" << endl;
    }
    i++;
  }

  cout << sum << endl;

  auto l2 = readList("[[2]]");
  auto l6 = readList("[[6]]");
  flatList.push_back(l2);
  flatList.push_back(l6);

  auto comp = [](const NestedList &a, const NestedList &b) {
    return compare(a, b).value_or(false);
  };

  sort(flatList.begin(), flatList.end(), comp);
  auto index2 = find_if(flatList.begin(), flatList.end(), [l2](const NestedList x) {
    return !compare(x, l2).has_value();
  }) - flatList.begin() + 1;
  auto index6 = find_if(flatList.begin(), flatList.end(), [l6](const NestedList x) {
    return !compare(x, l6).has_value();
  }) - flatList.begin() + 1;

  cout << index2 * index6 << endl;
}
