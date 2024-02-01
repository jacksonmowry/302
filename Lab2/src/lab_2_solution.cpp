// Jackson Mowry
// Feb 01 2024
#include "lab_2.hpp"
#include <cfloat>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

unsigned long long whats_my_number_ll() {
  // Handle either extreme first
  if (am_i_greater_ll(1)) {

    return 0;
  }
  if (!am_i_greater_ll(numeric_limits<uint64_t>::max())) {
    return numeric_limits<uint64_t>::max();
  }
  uint64_t curr;
  uint64_t left = 0;
  uint64_t right = numeric_limits<uint64_t>::max();
  while (left <= right) {
    curr = ((right - left) / 2) + left;
    if (left == right || right - left == 1) {
      // At the end we can possible have 2 values, so we return the correct one
      if (am_i_greater_ll(curr + 1)) {
        return curr;
      } else {
        return curr + 1;
      }
    }
    if (am_i_greater_ll(curr)) {
      right = curr - 1;
    } else {
      // We may be equal to the desired value, so don't +1
      left = curr;
    }
  }
  return 0;
}

double whats_my_number_d() {
  // If we move less than DBL_EPSILON then we end
  double curr;
  double left = 0;
  double right = 10E9; // upper bound given
  while (left <= right) {
    curr = ((right - left) / 2) + left;
    if ((curr - left) < DBL_EPSILON || (right - curr) < DBL_EPSILON) {
      return curr;
    }
    if (am_i_greater_d(curr)) {
      right = curr - DBL_EPSILON;
    } else {
      left = curr + DBL_EPSILON;
    }
    if (left == right || (right - left) < DBL_EPSILON) {
      return curr;
    }
  }
  return 0;
}

unsigned int optimal() {
  double max = 0;
  uint32_t max_pos;
  uint32_t curr;
  uint32_t left = 0;
  uint32_t right = std::numeric_limits<uint32_t>::max();
  while (left <= right) {
    curr = ((right - left) / 2) + left;
    if (func(curr) > max) {
      max = func(curr);
      max_pos = curr;
    }
    double mid = func(curr);
    double next = func(curr + 1);
    if (mid < next) {
      left = curr + 1;
    } else {
      right = curr - 1;
    }
  }
  return max_pos;
}
