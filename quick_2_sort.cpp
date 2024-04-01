#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

void recursive_sort(vector<double> &v, int start, int size, int print) {
  if (size > 1 && print) {
    printf("S: %5d %5d %6s", start, size, "");
    for (size_t i = 0; i < v.size(); i++) {
      printf("%2.2f", v[i]);
      if (i != v.size() - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }
  if (size <= 1) {
    return;
  }
  if (size == 2) {
    if (v[start] > v[start + 1]) {
      swap(v[start], v[start + 1]);
    }
    if (print && start == 0 && (size_t)size == v.size()) {
      printf("                ");
      for (auto f : v) {
        printf(" %.2f", f);
      }
      printf("\n");
    }
    return;
  }
  double f = v[start];
  double m = v[start + (size / 2)];
  double l = v[start + size - 1];
  vector<double> median = {f, m, l};
  sort(median.begin(), median.end());
  double pivot = median[1];

  if (median[1] == f) {
    ;
  } else if (median[1] == m) {
    swap(v[start], v[start + (size / 2)]);
  } else {
    swap(v[start], v[start + size - 1]);
  }

  if (print) {
    printf("M: %5d %5d %5.2f ", start, size, pivot);
    for (size_t i = 0; i < v.size(); i++) {
      printf("%2.2f", v[i]);
      if (i != v.size() - 1) {
        printf(" ");
      }
    }
    printf("\n");
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
  int first_larger = start + 1;
  while (v[first_larger] < pivot) {
    first_larger++;
  }
  swap(v[start], v[first_larger - 1]);

  if (print) {
    printf("P: %5d %5d %5d ", start, size, first_larger - 1);
    for (size_t i = 0; i < v.size(); i++) {
      printf("%2.2f", v[i]);
      if (i != v.size() - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }

  int l_size = first_larger - 1;
  int r_size = (start + size) - first_larger;
  if (l_size < 2 && r_size < 2) {
    // return;
  }
  recursive_sort(v, start, first_larger - 1 - start, print);
  recursive_sort(v, first_larger, (start + size) - first_larger, print);

  if (print && start == 0 && (size_t)size == v.size()) {
    printf("                ");
    for (auto f : v) {
      printf(" %.2f", f);
    }
    printf("\n");
  }
}

// void recursive_sort(vector<double> &v, int start, int size, int print) {
void sort_doubles(vector<double> &v, bool print) {
  recursive_sort(v, 0, v.size(), print);
}

// int main() {
//   vector<double> v = {5.77, 5.26, 6.49, 0.43, 6.09, 4.95,
//                       3.15, 3.46, 7.55, 6.82, 6.69, 3.62};
//   recursive_sort(v, 0, v.size(), 0);
//   /* for (auto a : v) { */
//   /*   printf("%.0f ", a); */
//   /* } */
//   /* printf("\n"); */
// }
