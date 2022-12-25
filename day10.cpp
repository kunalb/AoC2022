#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <sstream>

using namespace std;

struct Instr {
  virtual void startTick(int& x) {};
  virtual bool endTick(int& x) { return true; };
  virtual ~Instr() {};
};

struct Noop: Instr {
  void startTick(int& x) override {}

  bool endTick(int& x) override {
    return true;
  }
};

struct AddX: Instr {
  int v;
  int cycle = 0;

  AddX(int v): v(v) {};

  void startTick(int& x) override {
    cycle += 1;
  }

  bool endTick(int& x) override {
    if (cycle == 2) {
      x += v;
      return true;
    }

    return false;
  }
};


unique_ptr<Instr> readInstr(string line) {
  stringstream linestream(line);
  string instr;
  linestream >> instr;

  if (instr == "noop") {
    return make_unique<Noop>();
  } else {
    int v;
    linestream >> v;
    return make_unique<AddX>(v);
  }
}

list<unique_ptr<Instr>> readProgram() {
  string line;
  list<unique_ptr<Instr>> program;

  while (getline(cin, line)) {
    program.push_back(readInstr(line));
  }

  return program;
}

void showCRT(const char crt[6][40]) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 40; j++) {
      cout << crt[i][j];
    }
    cout << endl;
  }
}

int process(list<unique_ptr<Instr>> program) {
  set<int> interestingCycles = {};
  for (int p = 20; p <= 220; p += 40) {
    interestingCycles.insert(p);
  }

  int cycle = 0;
  int x = 1;

  int sum = 0;

  char crt[6][40];
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 40; j++)
      crt[i][j] = '.';

  unique_ptr<Instr> instr = nullptr;
  while (!program.empty()) {
    if (instr == nullptr) {
      instr = move(program.front());
      program.pop_front();
    }

    int crtY = cycle / 40;
    int crtX = cycle % 40;

    instr->startTick(x);
    for (int k = x-1; k <= x+1; k++) {
      if (k == crtX) {
        crt[crtY][k] = '#';
      }
    }

    cycle += 1;

    if (interestingCycles.find(cycle) != interestingCycles.end()) {
      sum += cycle * x;
      interestingCycles.erase(cycle);
    }

    if (instr->endTick(x)) {
      instr = nullptr;
      if (program.empty()) {
        break;
      }
    }
  }

  showCRT(crt);
  return sum;
}

int main() {
  cout << process(readProgram()) << endl;
}
