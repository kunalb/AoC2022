#include <iostream>
#include <string>
#include <vector>


using namespace std;


struct Gum {
  int member;
  string val;
  vector<int> vals;

  Gum(int x, string val, vector<int> vals): member(x), val(val), vals(vals) {};

  operator string() const { return val; }
};

ostream &operator<<(ostream& str, Gum const& g) {
  str << g.val;
  return str;
}


void addVal(vector<Gum>& v, int x) {
  string test = to_string(x);

  vector<int> vx {x};
  auto& res = v.emplace_back(x, to_string(x), vx);
  cout << "res " << res << endl;
}


int main() {
  vector<Gum> gums;

  for (int i = 0; i < 10; i++) {
    addVal(gums, i);
  }

  for (int i = 0; i < 10; i++) {
    cout << gums[i].val << endl;
  }

  return 0;
}
