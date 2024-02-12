#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

bool walk(int r, int c, vector<vector<int>> aj, vector<int> path,
          vector<bool> seen, int pos, int end) {
  if (pos == end) {
    path.push_back(pos);
    for (auto a : path) {
      printf("PATH %d\n", a);
    }
    return true;
  }
  if (seen[pos]) {
    return false;
  }
  if (aj[pos].size() == 0) {
    return false;
  }

  seen[pos] = true;
  for (size_t edge = 0; edge < aj[pos].size(); edge++) {
    path.push_back(pos);
    if (walk(r, c, aj, path, seen, aj[pos][edge], end)) {
      return true;
    } else {
      path.pop_back();
    }
  }

  return false;
}

int main() {
  int c;
  int r;
  string trash;
  int first;
  int second;

  cin >> trash >> r >> trash >> c;
  printf("ROWS %d COLS %d\n", r, c);
  vector<vector<int>> aj_list(r * c);
  set<pair<int, int>> walls;

  while (cin >> trash >> first >> second) {
    walls.emplace(first, second);
    walls.emplace(second, first);
    printf("WALL %d %d\n", first, second);
  }

  for (size_t i = 0; i < r * c; i++) {
    // down
    if ((i / c) < r - 1 && walls.find(make_pair(i, i + c)) == walls.end()) {
      aj_list[i].push_back(i + c);
    }
    // right
    if ((i % c) < c - 1 && walls.find(make_pair(i, i + 1)) == walls.end()) {
      aj_list[i].push_back(i + 1);
    }
    // up
    if ((i / c) != 0 && walls.find(make_pair(i, i - c)) == walls.end()) {
      aj_list[i].push_back(i - c);
    }
    // left
    if ((i % c) > 0 && walls.find(make_pair(i, i - 1)) == walls.end()) {
      aj_list[i].push_back(i - 1);
    }
  }

  vector<bool> seen(r * c, false);
  vector<int> path;
  walk(r, c, aj_list, path, seen, 0, r * c - 1);
}
