#include <iostream>
#include <string>

using namespace std;

long score_game(const string& play) {
  int player1 = play[0] - 'A';
  int player2 = play[2] - 'X';

  if (player2 == player1) {
    return 3;
  } else if (((player2 - 1) + 3) % 3 == player1) {
    return 6;
  } else {
    return 0;
  }
}

long score_fin(const string& play) {
  int player1 = play[0] - 'A';
  if (play[2] == 'X') {
    return 1 + ((player1 - 1) + 3) % 3;
  } else if (play[2] == 'Y') {
    return player1 + 1 + 3;
  } else {
    return (1 + (player1 + 1) % 3) + 6;
  }
}

int main(int argv, char* argc[]) {
  long score = 0;
  long score2 = 0;
  for (string line; getline(cin, line);) {
    score += (line[2] - 'X') + 1 + score_game(line);
    score2 += score_fin(line);
  }
  cout << score << endl;
  cout << score2 << endl;
  return 0;
}
