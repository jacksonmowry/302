// Jackson Mowry
// Mon Feb 19 2024
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

class Choices {
public:
  vector<int> total;
  vector<int> choices;
  vector<vector<int>> output;
  void choose_k(int index, int k) {
    // base case
    if (k == 0) {
      output.push_back(choices);
      return;
    }
    // imposible case
    if ((size_t)k > total.size() - index) {
      return;
    }
    // actual work
    choices.push_back(total[index]);
    choose_k(index + 1, k - 1);
    choices.pop_back();

    // call recursively without adding total[index]
    choose_k(index + 1, k);
  }
};

class Perm {
public:
  vector<int> total;
  vector<int> output;
  size_t currentIndex;
  bool gen_perm() {
    int i = total.size() - 2;
    // Walk backwards to find an element with smaller to its right
    while (i >= 0 && total[i] >= total[i + 1]) {
      i--;
    }

    // If no such elements exists we are done
    if (i == -1) {
      return false;
    }

    int j = total.size() - 1;
    // Walk back from there to find next element greater than total[i]
    while (total[j] <= total[i]) {
      j--;
    }

    // Swap the two numbers we found
    swap(total[i], total[j]);

    // Reverse the range following the index i
    reverse(total.begin() + i + 1, total.end());

    return true;
  }
};

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "usage: matrix_enum w e x|h");
    exit(1);
  }
  int w = atoi(argv[1]);
  int e = atoi(argv[2]);
  char mode = argv[3][0];

  Perm p;
  for (int i = 0; i < w; i++) {
    p.total.push_back(i);
  }
  do {
    vector<int> permutation = p.total;
    Choices c;
    for (int row = 0; row < w; row++) {
      for (int col = 0; col < w; col++) {
        if (col == permutation[row]) {
          continue;
        } else {
          c.total.push_back((row * w) + col);
        }
      }
    }
    c.choose_k(0, e);

    if (mode == 'x') {
      for (auto i : c.output) {
        for (int row = 0; row < w; row++) {
          for (int col = 0; col < w; col++) {
            if (col == permutation[row]) {
              printf("X");
            } else if (find(i.begin(), i.end(), (row * w) + col) != i.end()) {
              printf("E");
            } else {
              printf(".");
            }
          }
          printf("\n");
        }
        printf("\n");
      }
    } else if (mode == 'h') {
      for (auto i : c.output) {
        for (int row = 0; row < w; row++) {
          int accum = 0;
          for (int col = 0; col < w; col++) {
            if (col == permutation[row]) {
              accum += (1 << col);
            } else if (find(i.begin(), i.end(), (row * w) + col) != i.end()) {
              accum += (1 << col);
            } else {
              continue;
            }
          }
          printf("%x\n", accum);
        }
        printf("\n");
      }
    }
  } while (p.gen_perm());
}
