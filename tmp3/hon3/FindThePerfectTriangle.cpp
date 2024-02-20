#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

multimap<int, pair<int, int>> enum_sides() {
  multimap<int, pair<int, int>> lengths;
  for (int x = 0; x < 3000; x++) {
    for (int y = 0; y < 3000; y++) {
      double dist = sqrt(x * x + y * y);
      if (dist == trunc(dist)) {
        lengths.emplace(trunc(dist), make_pair(x, y));
      }
    }
  }
  lengths.erase(lengths.begin());
  return lengths;
}

#define VIT(i, v) for (i = 0; i < v.size(); i++)

bool Is_Triangle_OK(int area, int perimeter, int a, int b, int c) {
  int q = perimeter / 2;
  return (q * (q - a) * (q - b) * (q - c) == area * area);
}

class FindThePerfectTriangle {
public:
  vector<int> constructTriangle(int area, int perimeter);
};

vector<int> FindThePerfectTriangle::constructTriangle(int area, int perimeter) {
  vector<int> rv;
  multimap<int, pair<int, int>> lengths = enum_sides();

  for (int a = 1; a < perimeter / 3; a++) {
    for (int b = a; b < perimeter; b++) {
      int c = perimeter - a - b;
      if (c >= b && Is_Triangle_OK(area, perimeter, a, b, c)) {
        // Find points that create this side length
        int x1, y1;
        x1 = 0;
        y1 = 0;
        auto it_pair_c = lengths.equal_range(c);
        for (auto it = it_pair_c.first; it != it_pair_c.second; ++it) {
          int x2, y2;
          x2 = it->second.first;
          y2 = it->second.second;
          auto it_pair_a = lengths.equal_range(a);
          for (auto it_a = it_pair_a.first; it_a != it_pair_a.second; ++it_a) {
            int x3, y3;
            x3 = it_a->second.first;
            y3 = it_a->second.second;
            if (((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2)) == b * b) {
              // valid
              return {x1, y1, x2, y2, x3, y3};
            } else if (((x3 - x2) * (x3 - x2) + (y3 + y2) * (y3 + y2)) ==
                       b * b) {
              return {x1, y1, x2, y2, x3, -y3};
            }
          }
        }
      }
    }
  }

  return rv;
}
