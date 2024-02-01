#include "lab_2.hpp"
#include <cfloat>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

unsigned long long whats_my_number_ll() {
  if (am_i_greater_ll(1)) {
    return 0;
  }
  uint64_t curr;
  uint64_t left = 0;
  uint64_t right = std::numeric_limits<uint64_t>::max();
  while (left <= right) {
    curr = ((right - left) / 2) + left;
    // printf("left: %p, mid: %p, right: %p\n", left, curr, right);
    if (left == right || right - left == 1) {
      return curr;
    }
    if (am_i_greater_ll(curr)) {
      right = curr - 1;
    } else {
      left = curr;
    }
  }
  return 0;
}

double whats_my_number_d() {
  // If we move less than 10^-9 end
  double curr;
  double left = 0;
  double right = 10E9;
  while (left <= right) {
    curr = ((right - left) / 2) + left;
    printf("left: %.10f, mid: %.10f, right: %.10f\n", left, curr, right);
    if (am_i_greater_d(curr)) {
      right = curr - DBL_EPSILON;
    } else {
      left = curr + DBL_EPSILON;
    }
    if (left == right || (right - left) < 10E-10) {
      printf("returning %.10f\n", curr);
      return curr;
    }
  }
  cout << "returning 0\n";
  return 0;
}

unsigned int optimal() { return 0; }
