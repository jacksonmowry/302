#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

void recursive_sort(vector<double> &v, int start, int size, int print) {
  if (size <= 2) {
    return;
  }
  double f = v[start];
  double m = v[start + (size / 2)];
  double l = v[start + size - 1];
  vector<double> median = {f, m, l};
  sort(median.begin(), median.end());
  printf("first: %f, middle: %f, last: %f, median: %f\n", f, m, l, median[1]);
  double pivot = median[1];

  if (median[1] == f) {
    ;
  } else if (median[1] == m) {
    swap(v[start], v[start + (size / 2)]);
  } else {
    swap(v[start], v[start + size - 1]);
  }

  int left = start + 1;
  int right = start + size - 1;

  while (left < right) {
    while (left != start + size && v[left] < pivot) {
      left++;
    }
    while (right != start && v[right] > pivot) {
      right--;
    }
    if (left > right) {
      break;
    }
    swap(v[left], v[right]);
    left++;
    right--;
  }
  int first_larger;
}

int main() {
  vector<double> v = {3, 2, 4, 9, 8, 5, 1};
  recursive_sort(v, 0, v.size(), 0);
  for (auto a : v) {
    printf("%.0f ", a);
  }
  printf("\n");
}
