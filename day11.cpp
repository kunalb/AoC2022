#include <functional>
#include <iostream>
#include <list>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

using Worry = unsigned long;

struct Monkey {
  size_t id;
  list<Worry> items;
  function<Worry(Worry)> op;
  int testVal;
  size_t trueThrow;
  size_t falseThrow;
  size_t inspected = 0;
};


Worry extractNumber(string line) {
  regex number("\\d+");
  smatch match;
  if (regex_search(line, match, number)) {
    return stol(match[0]);
  }
  throw new invalid_argument("Didn't find a number in: " + line);
}

list<Worry> extractNumbers(string line) {
  regex number("\\d+");
  smatch match;
  list<Worry> results;
  while (regex_search(line, match, number)) {
    results.push_back(stol(match[0]));
    line = match.suffix().str();
  }

  return results;
}

enum class IR { X, Add, Mul, Val };

function<Worry(Worry)> compile(string line) {
  auto pos = line.find("=");
  auto compute = line.substr(pos + 2);

  IR ir[3];

  ir[0] = IR::X;
  if (compute[4] == '*') {
    ir[1] = IR::Mul;
  } else if (compute[4] == '+') {
    ir[1] = IR::Add;
  } else {
    throw invalid_argument("Can't read " + compute);
  }

  auto last_token_pos = compute.rfind(" ");
  auto last_token = compute.substr(last_token_pos + 1);
  Worry val = 0;
  if (last_token == "old") {
    ir[2] = IR::X;
  } else {
    val = stol(last_token);
  }

  return [ir, val, compute](Worry x) {
    auto val1 = x;
    auto val2 = ir[2] == IR::X ? x : val;

    switch (ir[1]) {
    case IR::Add:
      return val1 + val2;
    case IR::Mul:
      return val1 * val2;
    default:
      throw invalid_argument("Can't read " + compute);
    }
  };
}


vector<Monkey> readMonkeyDetails() {
  vector<Monkey> monkeys;
  string line;


  while (!cin.eof()) {
    Monkey monkey;
    getline(cin, line);
    monkey.id = extractNumber(line);

    getline(cin, line);
    monkey.items = extractNumbers(line);

    getline(cin, line);
    monkey.op = compile(line);

    getline(cin, line);
    monkey.testVal = extractNumber(line);

    getline(cin, line);
    monkey.trueThrow = extractNumber(line);

    getline(cin, line);
    monkey.falseThrow = extractNumber(line);

    getline(cin, line);
    monkeys.push_back(monkey);
  }

  return monkeys;
}

void runRound(vector<Monkey>& monkeys, bool reduceWorry = true, long cap = 1) {
  for (auto& monkey : monkeys) {
//  cout << "Monkey " << monkey.id << ":" << endl;
    for (auto item : monkey.items) {
//    cout << "  Monkey inspects an item with a worry level of " << item << "." << endl;
      auto worry = monkey.op(item);
//    cout << "    Worry level is updated to " << worry << "." << endl;
      if (reduceWorry) {
        worry /= 3;
      } else {
        worry = worry % cap;
      }
//    cout << "    Worry level is dropped to " << worry << "." << endl;

      if (worry % monkey.testVal == 0) {
//      cout << "    Current worry level is divisible by " << monkey.testVal << endl;
//      cout << "    Item with worry level " << worry << " is thrown to monkey "
//           << monkey.trueThrow << "." << endl;
        monkeys[monkey.trueThrow].items.push_back(worry);
      } else {
//      cout << "    Current worry level is not divisible by " << monkey.testVal << endl;
//      cout << "    Item with worry level " << worry << " is thrown to monkey "
//           << monkey.falseThrow << "." << endl;
        monkeys[monkey.falseThrow].items.push_back(worry);
      }

      monkey.inspected += 1;
    }

    monkey.items.clear();
  }
}

size_t monkeyBusiness(const vector<Monkey>& monkeys) {
  size_t max[2] = {0, 0};

  for (auto monkey : monkeys) {
    if (monkey.inspected > max[0]) {
      max[1] = max[0];
      max[0] = monkey.inspected;
    } else if (monkey.inspected > max[1]) {
      max[1] = monkey.inspected;
    }
  }

  return max[0] * max[1];
}

int main() {
  auto monkeys = readMonkeyDetails();
  auto monkeys2 = monkeys;

  for (int i = 0; i < 20; i++) {
    runRound(monkeys);
  }
  cout << monkeyBusiness(monkeys) << endl;

  long cap = 1;
  for (auto monkey : monkeys)
    cap *= monkey.testVal;

  for (int i = 1; i <= 10000; i++) {
    runRound(monkeys2, false, cap);

    if (i % 1000 == 0 || i == 1 || i == 20) {
      // cout << "== After round " << i << " ==" << endl;
      // for (auto monkey : monkeys2) {
      //   cout << "Monkey " << monkey.id << " inspected items " << monkey.inspected << " times."
      //        << " " << (monkey.items.empty() ? 1337 : monkey.items.front())
      //        << endl;
      // }
    }
  }
  cout << monkeyBusiness(monkeys2) << endl;

  return 0;
}
