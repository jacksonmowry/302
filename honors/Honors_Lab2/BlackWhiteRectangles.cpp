// Jackson Mowry
// Mon Feb 19 2024
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct OgRectangle {
  uint16_t x1;
  uint16_t y1;
  uint16_t x2;
  uint16_t y2;
  uint16_t pattern;
};

struct Rectangle {
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  uint8_t pattern;
};

class BlackWhiteRectangles {
public:
  auto blackCount(vector<string> rectangles) -> int;
};

auto BlackWhiteRectangles::blackCount(vector<string> R) -> int {
  vector<OgRectangle> og;
  set<uint16_t> X;
  set<uint16_t> Y;
  for (auto s : R) {
    istringstream is(s);
    OgRectangle r;
    is >> r.x1 >> r.y1 >> r.x2 >> r.y2 >> r.pattern;
    assert(r.pattern <= 4 && r.pattern >= 1);
    og.push_back(r);
    X.insert(r.x1);
    X.insert(r.x2);
    Y.insert(r.y1);
    Y.insert(r.y2);
  }

  assert(X.size() > 0);
  assert(Y.size() > 0);
  vector<uint16_t> x(X.begin(), X.end());
  vector<uint16_t> y(Y.begin(), Y.end());

  vector<Rectangle> r;
  for (size_t i = 0; i < x.size() - 1; i++) {
    for (size_t j = 0; j < y.size() - 1; j++) {
      r.push_back({x[i], y[j], (uint16_t)(x[i + 1] - x[i]),
                   (uint16_t)(y[j + 1] - y[j]), 0});
    }
  }

  int total_set = 0;

  for (Rectangle &rec : r) {
    for (OgRectangle ogr : og) {
      if (rec.x < ogr.x1 || rec.x + rec.w > ogr.x2) {
        continue;
      } else if (rec.y < ogr.y1 || rec.y + rec.h > ogr.y2) {
        continue;
      }
      // 4 8
      // 1 2
      switch (ogr.pattern) {
      case 1:
        rec.pattern |= 0b1111;
        break;
      case 2: {
        if (ogr.y1 % 2 == 0) {
          // Starts on even row
          rec.pattern |= 0b1100;
        } else {
          // Starts on odd row
          rec.pattern |= 0b0011;
        }
      } break;
      case 3: {
        if (ogr.x1 % 2 == 0) {
          // Starts on even col
          rec.pattern |= 0b1010;
        } else {
          // Starts on odd col
          rec.pattern |= 0b0101;
        }
      } break;
      case 4: {
        if (ogr.x1 % 2 == ogr.y1 % 2) {
          // Basic pattern
          rec.pattern |= 0b1001;
        } else {
          // Flip Flop
          rec.pattern |= 0b0110;
        }
      } break;
      default: {
        assert(false);
      }
      }
    }
    int even_c = (rec.w + ((rec.x + 1) % 2)) / 2;
    int even_r = (rec.h + ((rec.y + 1) % 2)) / 2;
    int odd_c = (rec.w + (rec.x % 2)) / 2;
    int odd_r = (rec.h + (rec.y % 2)) / 2;

    if (rec.pattern & 0b0001) {
      total_set += even_c * even_r;
    }
    if (rec.pattern & 0b0010) {
      total_set += odd_c * even_r;
    }
    if (rec.pattern & 0b0100) {
      total_set += even_c * odd_r;
    }
    if (rec.pattern & 0b1000) {
      total_set += odd_c * odd_r;
    }
  }

  return total_set;
}
