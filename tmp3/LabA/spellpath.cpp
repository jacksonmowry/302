#include <iostream>
#include <vector>

using namespace std;

bool differ_one(char a, char b) { return (a + 1 == b || a - 1 == b); }

int max_path_len(vector<string>& puzzle, int row, int col, int cur_max) {
    char cur = puzzle[row][col];
    puzzle[row][col] = '-';
    int val = 0;
    // Adjacency Rules
    // UP
    if (row > 0 && differ_one(cur, puzzle[row - 1][col])) {
        val = max(val, max_path_len(puzzle, row - 1, col, cur_max));
    }
    // DOWN
    if (row < puzzle.size() - 1 && differ_one(cur, puzzle[row + 1][col])) {
        val = max(val, max_path_len(puzzle, row + 1, col, cur_max));
    }
    // LEFT
    if (col > 0 && differ_one(cur, puzzle[row][col - 1])) {
        val = max(val, max_path_len(puzzle, row, col - 1, cur_max));
    }
    // RIGHT
    if (col < puzzle[0].size() && differ_one(cur, puzzle[row][col + 1])) {
        val = max(val, max_path_len(puzzle, row, col + 1, cur_max));
    }
    if (row < puzzle.size() - 1) {
        // Even down Left
        if (col % 2 == 0 && col > 0 &&
            differ_one(cur, puzzle[row + 1][col - 1])) {
            val =
                max(val, max_path_len(puzzle, row + 1, col - 1, cur_max));
        }
        // Even down Right
        if (col % 2 == 0 && col < puzzle[0].size() - 1 &&
            differ_one(cur, puzzle[row + 1][col + 1])) {
            val =
                max(val, max_path_len(puzzle, row + 1, col + 1, cur_max));
        }
    }
    if (row > 0) {
        // Odd up Left
        if (col % 2 != 0 && col > 0 &&
            differ_one(cur, puzzle[row - 1][col - 1])) {
            val =
                max(val, max_path_len(puzzle, row - 1, col - 1, cur_max));
        }
        // Odd up Right
        if (col % 2 != 0 && col < puzzle[0].size() - 1 &&
            differ_one(cur, puzzle[row - 1][col + 1])) {
            val =
                max(val, max_path_len(puzzle, row - 1, col + 1, cur_max));
        }
    }

    puzzle[row][col] = cur;
    return max(cur_max, 1 + val);
}

int main() {
    vector<string> puzzle;
    string input;

    while (cin >> input) {
        puzzle.push_back(input);
    }

    for (auto a : puzzle) {
        cout << a << endl;
    }

    int max_seen = 0;

    for (size_t row = 0; row < puzzle.size(); row++) {
        for (size_t col = 0; col < puzzle[0].size(); col++) {
            max_seen = max(max_seen, max_path_len(puzzle, row, col, 0));
        }
    }

    cout << max_seen << endl;
}
