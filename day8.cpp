#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

size_t countVisibleTrees(const vector<vector<char>> &grid) {
  size_t rows = grid.size();
  size_t cols = grid[0].size();
  int curMax;

  set<tuple<int, int>> visible;
  for (int i = 0; i < rows; i++) {
    // Left to right
    visible.emplace(i, 0);
    curMax = grid[i][0];
    for (int j = 1; j < cols; j++) {
      if (grid[i][j] > curMax) {
        visible.emplace(i, j);
        curMax = grid[i][j];
      }
    }

    // Right to left
    visible.emplace(i, cols - 1);
    curMax = grid[i][cols - 1];
    for (int j = cols - 2; j >= 0; j--) {
      if (grid[i][j] > curMax) {
        visible.emplace(i, j);
        curMax = grid[i][j];
      }
    }
  }

  for (int j = 0; j < cols; j++) {
    // Top down
    visible.emplace(0, j);
    curMax = grid[0][j];
    for (int i = 1; i < rows; i++) {
      if (grid[i][j] > curMax) {
        visible.emplace(i, j);
        curMax = grid[i][j];
      }
    }

    // Bottom up
    visible.emplace(rows - 1, j);
    curMax = grid[rows - 1][j];
    for (int i = rows - 2; i >= 0; i--) {
      if (grid[i][j] > curMax) {
        visible.emplace(i, j);
        curMax = grid[i][j];
      }
    }
  }

  // for (int i = 0; i < rows; i++) {
  //   for (int j = 0; j < cols; j++) {
  //     if (visible.contains(make_tuple(i, j))) {
  //       cout << "x";
  //     } else {
  //       cout << ".";
  //     }
  //   }
  //   cout << endl;
  // }

  return visible.size();
}

size_t score(const vector<vector<char>> &grid, size_t rows, size_t cols,
             size_t i, size_t j) {
  size_t final = 1;

  const tuple<int, int> dirs[4] = {
    make_tuple(-1, 0),
    make_tuple(0, 1),
    make_tuple(1, 0),
    make_tuple(0, -1),
  };

  for (auto t: dirs) {
    int x = i, y = j;
    int xNext = x + get<0>(t);
    int yNext = y + get<1>(t);

    size_t cur = 0;
    while (xNext >= 0 && xNext <= rows - 1 &&
           yNext >= 0 && yNext <= cols - 1) {
      cur++;

      if (grid[i][j] <= grid[xNext][yNext])
        break;

      x = xNext;
      y = yNext;

      xNext += get<0>(t);
      yNext += get<1>(t);
    }

    final *= cur;
  }

  return final;
}

size_t bestScore(const vector<vector<char>>& grid) {
  size_t rows = grid.size();
  size_t cols = grid[0].size();
  size_t maxScore = 0;

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      auto curScore = score(grid, rows, cols, i, j);
      // cout << curScore << " ";
      if (maxScore < curScore) {
        maxScore = curScore;
      }
    }
    // cout << endl;
  }

  return maxScore;
}

int main() {
  vector<vector<char>> grid;

  string line;
  for (int i = 0; getline(cin, line); i++) {
    grid.push_back(vector<char>());
    for (int j = 0; j < line.size(); j++)
      grid[i].push_back(line[j] - '0');
  }

  cout << countVisibleTrees(grid) << endl;
  cout << bestScore(grid) << endl;
}
