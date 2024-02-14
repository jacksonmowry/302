#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Answer {
  public:
    int length;
    int r;
    int c;
    string key;
    Answer* nexta;
};

class SpellSeeker {
  public:
    vector<string> grid;
    map<string, Answer*> cache;
    int total_r;
    int total_c;

    Answer* Solve(int r, int c);
};

bool differ_one(char a, char b) { return (a + 1 == b || a - 1 == b); }

void dfs(vector<string>& puzzle, int row, int col, string& key) {
    key[(row * puzzle[0].size()) + col] =
        (key[(row * puzzle[0].size()) + col] == 'X') ? 'X' : 'O';
    char cur = puzzle[row][col];
    puzzle[row][col] = '-';
    int val = 0;
    // Adjacency Rules
    // UP
    if (row > 0 && differ_one(cur, puzzle[row - 1][col])) {
        dfs(puzzle, row - 1, col, key);
    }
    // DOWN
    if (row < puzzle.size() - 1 && differ_one(cur, puzzle[row + 1][col])) {
        dfs(puzzle, row + 1, col, key);
    }
    // LEFT
    if (col > 0 && differ_one(cur, puzzle[row][col - 1])) {
        dfs(puzzle, row, col - 1, key);
    }
    // RIGHT
    if (col < puzzle[0].size() && differ_one(cur, puzzle[row][col + 1])) {
        dfs(puzzle, row, col + 1, key);
    }
    if (row < puzzle.size() - 1) {
        // Even down Left
        if (col % 2 == 0 && col > 0 &&
            differ_one(cur, puzzle[row + 1][col - 1])) {
            dfs(puzzle, row + 1, col - 1, key);
        }
        // Even down Right
        if (col % 2 == 0 && col < puzzle[0].size() - 1 &&
            differ_one(cur, puzzle[row + 1][col + 1])) {

            dfs(puzzle, row + 1, col + 1, key);
        }
    }
    if (row > 0) {
        // Odd up Left
        if (col % 2 != 0 && col > 0 &&
            differ_one(cur, puzzle[row - 1][col - 1])) {
            dfs(puzzle, row - 1, col - 1, key);
        }
        // Odd up Right
        if (col % 2 != 0 && col < puzzle[0].size() - 1 &&
            differ_one(cur, puzzle[row - 1][col + 1])) {
            dfs(puzzle, row - 1, col + 1, key);
        }
    }

    puzzle[row][col] = cur;
}

Answer* SpellSeeker::Solve(int row, int col) {
    string key(total_r * total_c, '-');
    key[(row * total_c) + col] = 'X';
    dfs(grid, row, col, key);
    map<string, Answer*>::iterator it;
    if ((it = cache.find(key)) != cache.end()) {
        return it->second;
    }

    Answer* a = new Answer{1, row, col, key, nullptr};

    char cur = grid[row][col];
    grid[row][col] = '-';
    int val = 0;

    // Adjacency Rules
    // UP
    if (row > 0 && differ_one(cur, grid[row - 1][col])) {
        Answer* b = Solve(row - 1, col);
        if (b->length + 1 > a->length) {
            a->nexta = b;
            a->length = b->length + 1;
        }
    }
    // DOWN
    if (row < grid.size() - 1 && differ_one(cur, grid[row + 1][col])) {
        Answer* b = Solve(row + 1, col);
        if (b->length + 1 > a->length) {
            a->nexta = b;
            a->length = b->length + 1;
        }
    }
    // LEFT
    if (col > 0 && differ_one(cur, grid[row][col - 1])) {
        Answer* b = Solve(row, col - 1);
        if (b->length + 1 > a->length) {
            a->nexta = b;
            a->length = b->length + 1;
        }
    }
    // RIGHT
    if (col < grid[0].size() && differ_one(cur, grid[row][col + 1])) {
        Answer* b = Solve(row, col + 1);
        if (b->length + 1 > a->length) {
            a->nexta = b;
            a->length = b->length + 1;
        }
    }
    if (row < grid.size() - 1) {
        // Even down Left
        if (col % 2 == 0 && col > 0 &&
            differ_one(cur, grid[row + 1][col - 1])) {
            Answer* b = Solve(row + 1, col - 1);
            if (b->length + 1 > a->length) {
                a->nexta = b;
                a->length = b->length + 1;
            }
        }
        // Even down Right
        if (col % 2 == 0 && col < grid[0].size() - 1 &&
            differ_one(cur, grid[row + 1][col + 1])) {
            Answer* b = Solve(row + 1, col + 1);
            if (b->length + 1 > a->length) {
                a->nexta = b;
                a->length = b->length + 1;
            }
        }
    }
    if (row > 0) {
        // Odd up Left
        if (col % 2 != 0 && col > 0 &&
            differ_one(cur, grid[row - 1][col - 1])) {
            Answer* b = Solve(row - 1, col - 1);
            if (b->length + 1 > a->length) {
                a->nexta = b;
                a->length = b->length + 1;
            }
        }
        // Odd up Right
        if (col % 2 != 0 && col < grid[0].size() - 1 &&
            differ_one(cur, grid[row - 1][col + 1])) {
            Answer* b = Solve(row - 1, col + 1);
            if (b->length + 1 > a->length) {
                a->nexta = b;
                a->length = b->length + 1;
            }
        }
    }

    grid[row][col] = cur;
    cache.insert(make_pair(a->key, a));
    // cout << a->key << endl;
    // puts("at end");
    return a;
}

int main() {
    SpellSeeker puzzle;
    string input;

    while (cin >> input) {
        puzzle.grid.push_back(input);
    }

    puzzle.total_r = puzzle.grid.size();
    puzzle.total_c = puzzle.grid[0].size();

    for (auto a : puzzle.grid) {
        cout << a << endl;
    }

    Answer* largest = nullptr;

    for (size_t row = 0; row < puzzle.grid.size(); row++) {
        for (size_t col = 0; col < puzzle.grid[0].size(); col++) {
            Answer* a = puzzle.Solve(row, col);
            if (!largest || a->length >= largest->length) {
                largest = a;
            }
        }
    }

    cout << "PATH" << endl;

    while (largest) {
        cout << largest->r << ' ' << largest->c << endl;
        largest = largest->nexta;
    }

    for (auto it = puzzle.cache.begin(); it != puzzle.cache.end(); it++) {
        delete it->second;
    }
}
