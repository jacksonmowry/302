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
    int i;
    // base case
    if (k == 0) {
      // printf("base case: ");
      // for (auto i : choices) {
      //   printf("%d ", i);
      // }
      // printf("\n");
      output.push_back(choices);
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
  void gen_perm(int index) {
    int i;
    if (index == total.size()) {
      output.push_back(total);
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
  for (int i = 0; i < w; i++) {
    p.total.push_back(i);
  }
  p.gen_perm(0);
  // for (auto v : p.output) {
  //   for (auto i : v) {
  //     printf("%d ", i);
  //   }
  //   printf("\n");
  // }
  for (vector<int> permutation : p.output) {
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
    // printf("choices: ");
    // for (auto i : c.output) {
    //   printf("[");
    //   for (auto v : i) {
    //     printf("%d ", v);
    //   }
    //   printf("]");
    // }
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
          printf("%d\n", accum);
        }
        printf("\n");
      }
    }
  }
}
