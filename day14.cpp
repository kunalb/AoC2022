#include <cassert>
#include <complex>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>

using namespace std;
using namespace std::complex_literals;

using Pt = complex<int>;

constexpr bool complex_compare(const Pt &a, const Pt &b) {
  if (real(a) < real(b))
    return true;
  else if (real(a) > real(b))
    return false;

  return imag(a) < imag(b);
}

struct Grid {
  // Index points with complex numbers:
  //   y = imag
  //   x = real

  map<Pt, char, decltype(&complex_compare)> data;
  Pt topRight;
  Pt bottomLeft;

  Grid()
      : data(complex_compare),
        topRight(numeric_limits<int>::max(), numeric_limits<int>::max()),
        bottomLeft(numeric_limits<int>::min(), numeric_limits<int>::min()) {}
  ~Grid() { data.clear(); }

  char &operator[](const Pt &pt) {
    updateLimits(pt);
    return data[pt];
  }

  void addLine(Pt a, Pt b) {
    updateLimits(a);
    updateLimits(b);
    auto d = (b - a) / abs(b - a);

    Pt cur = a;
    data[cur] = '#';
    do {
      cur += d;
      data[cur] = '#';
    } while (cur != b);
  }

  void updateLimits(const Pt &x) {
    topRight.real(min(topRight.real(), x.real()));
    topRight.imag(min(topRight.imag(), x.imag()));

    bottomLeft.real(max(bottomLeft.real(), x.real()));
    bottomLeft.imag(max(bottomLeft.imag(), x.imag()));
  }

  void display() {
    for (int j = topRight.imag() - 1; j <= bottomLeft.imag() + 1; j++) {
      for (int i = topRight.real() - 1; i <= bottomLeft.real() + 1; i++) {
        auto it = data.find(complex(i, j));
        if (it == data.end()) {
          cout << ".";
        } else {
          cout << it->second;
        }
      }
      cout << endl;
    }

    cout << endl;
  }
};

void fillGrid(Grid &grid) {
  string line;
  while (getline(cin, line)) {
    stringstream linestream(line);

    bool isFirst = true;
    Pt prev, cur;
    while (!linestream.eof()) {
      if (!isFirst) {
        assert(linestream.get() == ' ');
        assert(linestream.get() == '-');
        assert(linestream.get() == '>');
        assert(linestream.get() == ' ');
      }

      int x;
      linestream >> x;
      assert(linestream.get() == ',');
      int y;
      linestream >> y;

      prev = cur;
      cur = complex(x, y);
      if (!isFirst) {
        grid.addLine(prev, cur);
      }

      isFirst = false;
    }
  }

  grid[complex(500, 0)] = '+';
}

bool pourSand(Grid &grid) {
  Pt start = complex(500, 0);
  bool moving = true;

  while (start.imag() <= grid.bottomLeft.imag() + 1 && moving) {
    moving = false;

    Pt nextPt = complex(start.real(), start.imag() + 1);
    if (grid.data.find(nextPt) == grid.data.end()) {
      goto foundPt;
    }

    nextPt.real(nextPt.real() - 1);
    if (grid.data.find(nextPt) == grid.data.end()) {
      goto foundPt;
    }

    nextPt.real(nextPt.real() + 2);
    if (grid.data.find(nextPt) == grid.data.end()) {
      goto foundPt;
    }

    grid[start] = 'o';
    return true;

  foundPt:
    start = nextPt;
    moving = true;
  }

  return false;
}

bool pourSandWithFloor(Grid &grid, int floor) {
  Pt start = complex(500, 0);
  if (grid[start] != '+') {
    return false;
  }

  while (true) {
    Pt nextPt = complex(start.real(), start.imag() + 1);

    if (nextPt.imag() != floor) {
      if (grid.data.find(nextPt) == grid.data.end()) {
        goto foundPt;
      }

      nextPt.real(nextPt.real() - 1);
      if (grid.data.find(nextPt) == grid.data.end()) {
        goto foundPt;
      }

      nextPt.real(nextPt.real() + 2);
      if (grid.data.find(nextPt) == grid.data.end()) {
        goto foundPt;
      }
    }

    grid[start] = 'o';
    return true;
  foundPt:
    start = nextPt;
  }

  return false;
}

int main() {
  Grid grid;
  fillGrid(grid);

  Grid gridWithFloor = grid;

  int sand = 0;
  while (pourSand(grid))
    sand++;

  grid.display();
  cout << sand << endl;

  int sandWithFloor = 0;
  int floor = gridWithFloor.bottomLeft.imag() + 2;
  while (pourSandWithFloor(gridWithFloor, floor))
    sandWithFloor++;

  gridWithFloor.display();
  cout << sandWithFloor << endl;
}
