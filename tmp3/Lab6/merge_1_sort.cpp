#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

void recursive_sort(vector<double> &v, vector<double> &temp, int start,
                    int size, int print) {
  if (size > 1) {
    printf("B: %5d %5d %5s ", start, size, "");
    for (auto f : v) {
      printf(" %.2f", f);
    }
    printf("\n");
  }
  if (size <= 1) {
    return;
  }
  if (size == 2) {
    if (v[start] > v[start + 1]) {
      swap(v[start], v[start + 1]);
      return;
    }
  }

  int l_size = size / 2;
  int r_size = size - (size / 2);
  recursive_sort(v, temp, start, l_size, 0);
  recursive_sort(v, temp, start + (size / 2), r_size, 0);

  // merge
  int l = start;
  int l_start = l;
  int r = start + (size / 2);
  int r_start = r;
  while (l < (l_start + l_size) && r < (r_start + r_size)) {
    if (v[l] < v[r]) {
      temp.push_back(v[l++]);
    } else {
      temp.push_back(v[r++]);
    }
  }
  while (l != l_start + l_size) {
    temp.push_back(v[l++]);
  }
  while (r != (r_start + r_size)) {
    temp.push_back(v[r++]);
  }
  for (size_t i = 0; i < temp.size(); i++) {
    v[start + i] = temp[i];
  }
  temp.clear();
  printf("E: %5d %5d %5s ", start, size, "");
  for (auto f : v) {
    printf(" %.2f", f);
  }
  printf("\n");

  if (start == 0 && size == v.size()) {
    printf("%21s", "");
    for (auto f : v) {
      printf(" %.2f", f);
    }
    printf("\n");
  }
}

int main() {
  vector<double> v = {0.42, 4.54, 8.35, 3.36, 5.65, 0.02, 1.88, 9.90,
                      7.50, 3.66, 3.51, 5.73, 1.33, 0.64, 9.51, 1.54};
  vector<double> tmp;
  recursive_sort(v, tmp, 0, v.size(), 0);
}
