#include <cmath>
#include <exception>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

enum Dir { L = 'L', R = 'R', U = 'U', D = 'D' };

struct Step {
  Dir dir;
  int count;

  Step(Dir dir, int count) : dir(dir), count(count) {}
};

struct Pt {
  int x = 0;
  int y = 0;

  Pt(int x, int y) : x(x), y(y) {}
  Pt() : Pt(0, 0) {}

  bool operator<(const Pt &rhs) const { return x < rhs.x && y < rhs.y; }

  tuple<int, int> to_tuple() { return make_tuple(x, y); }
};

ostream &operator<<(ostream &os, const Pt &pt) {
  os << '(' << pt.x << ',' << pt.y << ')';
  return os;
}

Pt getDelta(Dir dir) {
  switch (dir) {
  case L:
    return Pt(0, -1);
  case R:
    return Pt(0, 1);
  case U:
    return Pt(-1, 0);
  case D:
    return Pt(1, 0);
  default:
    throw runtime_error("Unexpected direction: " + string(1, (char)dir));
  }
}

list<Step> readMotions() {
  list<Step> steps;
  string line;

  while (getline(cin, line)) {
    stringstream str(line);
    char dir;
    int count;

    str >> dir;
    str >> count;

    steps.emplace_back(static_cast<Dir>(dir), count);
  }

  return steps;
}

int simulateRope(list<Step> steps, int totalKnots = 2) {
  set<tuple<int, int>> tailPoints;
  vector<Pt> knots(totalKnots);
  for (int i = 0; i < totalKnots; i++)
    knots[i] = Pt(0, 0);

  tailPoints.insert(knots.back().to_tuple());

  for (auto step : steps) {
    Pt delta = getDelta(step.dir);
    for (int i = 0; i < step.count; i++) {
      knots[0].x += delta.x;
      knots[0].y += delta.y;

      for (int k = 1; k < totalKnots; k++) {
        if (abs(knots[k - 1].x - knots[k].x) <= 1 &&
            abs(knots[k - 1].y - knots[k].y) <= 1) {
          // No-op, already adjacent
        } else {
          if (knots[k].y != knots[k - 1].y) {
            knots[k].y += (knots[k - 1].y - knots[k].y) /
                          abs(knots[k - 1].y - knots[k].y);
          }

          if (knots[k].x != knots[k - 1].x) {
            knots[k].x += (knots[k - 1].x - knots[k].x) /
                          abs(knots[k - 1].x - knots[k].x);
          }
        }
      }

      tailPoints.insert(knots.back().to_tuple());
    }
  }

  return tailPoints.size();
}

int main() {
  auto steps = readMotions();
  cout << simulateRope(steps, 2) << endl;
  cout << simulateRope(steps, 10) << endl;
}
