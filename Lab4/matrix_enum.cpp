#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

class Choices {
public:
  vector<int> permutation;
  vector<int> total;
  vector<int> choices;
  vector<vector<int>> output;
  int w;
  int e;
  char mode;
  void choose_k(int index, int k) {
    int i;
    // base case
    if (k == 0) {
      if (mode == 'x') {
        for (int row = 0; row < w; row++) {
          for (int col = 0; col < w; col++) {
            if (col == permutation[row]) {
              printf("X");
            } else if (find(choices.begin(), choices.end(), (row * w) + col) !=
                       choices.end()) {
              printf("E");
            } else {
              printf(".");
            }
          }
          printf("\n");
        }
        printf("\n");
      } else if (mode == 'h') {
        for (int row = 0; row < w; row++) {
          int accum = 0;
          for (int col = 0; col < w; col++) {
            if (col == permutation[row]) {
              accum += (1 << col);
            } else if (find(choices.begin(), choices.end(), (row * w) + col) !=
                       choices.end()) {
              accum += (1 << col);
            } else {
              continue;
            }
          }
          printf("%x\n", accum);
        }
        printf("\n");
      } else {
        puts("bad mode");
      }
      return;
    }
    // imposible case
    if (k > total.size() - index) {
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
  vector<vector<int>> output;
  int w;
  int e;
  char mode;
  void gen_perm(int index) {
    int i;
    if (index == total.size()) {
      Choices c;
      c.permutation = total;
      c.w = w;
      c.e = e;
      c.mode = mode;
      for (int row = 0; row < w; row++) {
        for (int col = 0; col < w; col++) {
          if (col == total[row]) {
            continue;
          } else {
            c.total.push_back((row * w) + col);
          }
        }
      }
      c.choose_k(0, e);
      return;
    }
    for (i = index; i < total.size(); i++) {
      swap(total[i], total[index]);
      gen_perm(index + 1);
      swap(total[i], total[index]);
    }
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
  p.w = w;
  p.e = e;
  p.mode = mode;
  for (int i = 0; i < w; i++) {
    p.total.push_back(i);
  }
  p.gen_perm(0);
}
