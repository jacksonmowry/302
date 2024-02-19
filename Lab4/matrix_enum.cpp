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
      // printf("base case: ");
      // for (auto i : choices) {
      //   printf("%d ", i);
      // }
      // printf("\n");
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
  // void gen_perm(int index) {
  //   size_t i;
  //   if ((size_t)index == total.size()) {
  //     output.push_back(total);
  //     return;
  //   }
  //   for (i = index; i < total.size(); i++) {
  //     swap(total[i], total[index]);
  //     gen_perm(index + 1);
  //     swap(total[i], total[index]);
  //   }
  // }

  void gen_perm() {
    size_t n = total.size();
    if (currentIndex == n) {
      output = total;
      return;
    }

    for (size_t i = currentIndex; i < n; i++) {
      std::swap(total[i], total[currentIndex]);
      currentIndex++;
      return; // Return after the first swap for now, to generate one
              // permutation at a time
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
  // p.gen_perm(0);
  // while (p.gen_perm()) {
  //   ;
  // }
  while (p.currentIndex != p.total.size()) {
    p.gen_perm();
    vector<int> permutation = p.output;
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
  }
}
