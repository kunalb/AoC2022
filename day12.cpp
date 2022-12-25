#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace std;

struct Pt {
  int x;
  int y;

  size_t dist;

  Pt() : x(0), y(0) {}
  Pt(size_t x, size_t y) : x(x), y(y) {}

  tuple<size_t, size_t> asTuple() { return make_tuple(x, y); }
};

struct Grid {
  size_t cols;
  size_t rows;
  vector<char> data;

  Grid() : cols(0), rows(0) {}

  Grid(size_t rows, size_t cols, char ch = 0)
      : cols(cols), rows(rows), data(rows * cols) {
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        at(i, j) = ch;
  }

  void print() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        cout << data[i * cols + j];
      }
      cout << endl;
    }
  }

  char &at(size_t i, size_t j) { return data[i * cols + j]; }
  char at(size_t i, size_t j) const { return data[i * cols + j]; }

  char &at(Pt pt) { return at(pt.x, pt.y); }
  char at(Pt pt) const { return at(pt.x, pt.y); }
};

tuple<Grid, Pt, Pt> readGrid() {
  string line;
  Grid grid;
  Pt start, end;

  while (getline(cin, line)) {
    if (grid.cols == 0) {
      grid.cols = line.size();
    }

    size_t col = 0;
    for (char ch : line) {
      if (ch == 'S') {
        start.x = grid.rows;
        start.y = col;
        ch = 'a';
      } else if (ch == 'E') {
        end.x = grid.rows;
        end.y = col;
        ch = 'z';
      }

      grid.data.push_back(ch);
      col++;
    }

    grid.rows++;
  }

  return make_tuple(grid, start, end);
}

ulong shortestPath(const Grid &grid, Pt start, Pt end) {
  set<tuple<uint, uint>> visited;

  start.dist = 0;

  auto cmp = [](Pt a, Pt b) { return a.dist > b.dist; };
  vector<Pt> wavefront = {start};
  make_heap(wavefront.begin(), wavefront.end(), cmp);

  while (!wavefront.empty()) {
    pop_heap(wavefront.begin(), wavefront.end(), cmp);
    Pt cur = wavefront.back();
    wavefront.pop_back();

    // cout << cur.x << " " << cur.y << " " << cur.dist << endl;

    if (visited.find(cur.asTuple()) != visited.end())
      continue;
    visited.insert(cur.asTuple());

    auto deltas = {make_tuple(0, 1), make_tuple(0, -1), make_tuple(1, 0),
                   make_tuple(-1, 0)};
    for (auto delta : deltas) {
      Pt pt = Pt(cur.x + get<0>(delta), cur.y + get<1>(delta));

      if (!(pt.x >= 0 && pt.x < grid.rows && pt.y >= 0 && pt.y < grid.cols))
        continue;

      auto z = grid.at(pt) - grid.at(cur);
      if (z > 1)
        continue;

      pt.dist = cur.dist + 1;
      // cout << "> " << pt.x << " " << pt.y << " " << pt.dist << endl;

      if (pt.x == end.x && pt.y == end.y)
        return pt.dist;

      wavefront.push_back(pt);
      push_heap(wavefront.begin(), wavefront.end(), cmp);
    }

    // grid.print();
    // cout << endl;
  }

  throw invalid_argument("Unexpected end of function");
}

ulong allShortestPath(const Grid &grid, Pt end) {
  set<tuple<uint, uint>> visited;

  size_t minDist = SIZE_MAX;
  end.dist = 0;

  auto cmp = [](Pt a, Pt b) { return a.dist > b.dist; };
  vector<Pt> wavefront = {end};
  make_heap(wavefront.begin(), wavefront.end(), cmp);

  while (!wavefront.empty()) {
    pop_heap(wavefront.begin(), wavefront.end(), cmp);
    Pt cur = wavefront.back();
    wavefront.pop_back();

    if (visited.find(cur.asTuple()) != visited.end())
      continue;
    visited.insert(cur.asTuple());

    auto deltas = {make_tuple(0, 1), make_tuple(0, -1), make_tuple(1, 0),
                   make_tuple(-1, 0)};
    for (auto delta : deltas) {
      Pt pt = Pt(cur.x + get<0>(delta), cur.y + get<1>(delta));

      if (!(pt.x >= 0 && pt.x < grid.rows && pt.y >= 0 && pt.y < grid.cols))
        continue;

      auto z = grid.at(cur) - grid.at(pt);
      if (z > 1)
        continue;

      pt.dist = cur.dist + 1;

      if (grid.at(pt) == 'a')
        minDist = min(minDist, pt.dist);

      wavefront.push_back(pt);
      push_heap(wavefront.begin(), wavefront.end(), cmp);
    }
  }

  return minDist;
}

int main() {
  Grid grid;
  Pt start, end;
  tie(grid, start, end) = readGrid();

  cout << shortestPath(grid, start, end) << endl;
  cout << allShortestPath(grid, end) << endl;
}
