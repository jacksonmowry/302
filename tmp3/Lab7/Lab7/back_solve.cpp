#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2; // Combining the hash values
  }
};

bool walk(int rows, int cols, unordered_set<pair<int, int>, PairHash> walls,
          vector<int> &path, vector<bool> &seen, int pos, int end) {
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
  // printf("pos: %d\n", pos);
  seen[pos] = true;
  // up
  if (pos / cols > 0 && walls.find(make_pair(pos, pos - cols)) == walls.end()) {
    // printf("new pos in up: %d\n", pos - cols);
    path.push_back(pos);
    if (walk(rows, cols, walls, path, seen, pos - cols, end)) {
      return true;
    } else {
      path.pop_back();
    }
  }
  // down
  if (pos / cols < rows - 1 &&
      walls.find(make_pair(pos, pos + cols)) == walls.end()) {
    // printf("new pos in down: %d\n", pos + cols);
    path.push_back(pos);
    if (walk(rows, cols, walls, path, seen, pos + cols, end)) {
      return true;
    } else {
      path.pop_back();
    }
  }
  // left
  if (pos % cols > 0 && walls.find(make_pair(pos, pos - 1)) == walls.end()) {
    // printf("new pos in left: %d\n", pos - 1);
    path.push_back(pos);
    if (walk(rows, cols, walls, path, seen, pos - 1, end)) {
      return true;
    } else {
      path.pop_back();
    }
  }
  // right
  if (pos % cols < cols - 1 &&
      walls.find(make_pair(pos, pos + 1)) == walls.end()) {
    // printf("new pos in right: %d\n", pos + 1);
    path.push_back(pos);
    if (walk(rows, cols, walls, path, seen, pos + 1, end)) {
      return true;
    } else {
      path.pop_back();
    }
  }

  return false;
}

int main() {
  int rows;
  int cols;
  string junk;
  cin >> junk >> rows >> junk >> cols;
  printf("ROWS %d COLS %d\n", rows, cols);
  unordered_set<pair<int, int>, PairHash> walls;
  int first;
  int second;
  while (cin >> junk >> first >> second) {
    printf("WALL %d %d\n", first, second);
    walls.emplace(first, second);
    walls.emplace(second, first);
  }

  int pos = 0;
  int end = rows * cols - 1;

  vector<int> path;
  vector<bool> seen(rows * cols, false);

  walk(rows, cols, walls, path, seen, pos, end);
}
