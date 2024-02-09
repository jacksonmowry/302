#include <algorithm>
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

struct Rect {
  int x1;
  int y1;
  int x2;
  int y2;
  int pattern;
};

class BlackWhiteRectangles {
public:
  int blackCount(vector<string>);
};

int BlackWhiteRectangles::blackCount(vector<string> R) {
  vector<Rect> og_rects;
  set<int> x;
  set<int> y;

  for (auto s : R) {
    // printf("%s\n", s.c_str());
    stringstream ss(s);
    Rect og;
    ss >> og.x1 >> og.y1 >> og.x2 >> og.y2 >> og.pattern;
    x.insert(og.x1);
    y.insert(og.y1);
    x.insert(og.x2);
    y.insert(og.y2);
    og_rects.push_back(og);
  }

  vector<int> xs(x.begin(), x.end());
  vector<int> ys(y.begin(), y.end());
  for (size_t i = 0; i < xs.size() - 1; i++) {
    for (size_t j = 0; j < ys.size() - 1; j++) {
      // printf("Rect: x:%d, y:%d, w:%d, h:%d\n", xs[i], ys[j], xs[i + 1] -
      // xs[i],
      //        ys[j + 1] - ys[j]);
      int pattern = 0;
      for (size_t r = 0; r < og_rects.size(); r++) {
        if (xs[i] < og_rects[r].x1 || xs[i - 1] - xs[i] > og_rects[r].x2 ||
            ys[j] < og_rects[r].y1 || ys[j - 1] - ys[j] > og_rects[r].y2) {
          continue;
        }
        if (og_rects[r].x1 % 2 == 0) {
          // Even column
          if (og_rects[r].y1 % 2 == 0) {
            // Even rows
            if (og_rects[r].pattern == 1) {
              pattern |= 0xf;
            } else if (og_rects[r].pattern == 2) {
              pattern |= 0xc;
            } else if (og_rects[r].pattern == 3) {
              pattern |= 0x6;
            } else {
              pattern |= 0x9;
            }
          } else {
            // odd rows
            if (og_rects[r].pattern == 1) {
              pattern |= 0xf;
            } else if (og_rects[r].pattern == 2) {
              pattern |= 0x3;
            } else if (og_rects[r].pattern == 3) {
              pattern |= 0x6;
            } else {
              pattern |= 0xa;
            }
          }
        } else {
          // 4 8
          // 1 2
          // Odd columns
          if (og_rects[r].y1 % 2 == 0) {
            // even rows
            if (og_rects[r].pattern == 1) {
              pattern |= 0xf;
            } else if (og_rects[r].pattern == 2) { // odd row set
              pattern |= 0xc;
            } else if (og_rects[r].pattern == 3) {
              pattern |= 0x5;
            } else {
              pattern |= 0x6;
            }
          } else {
            // odd rows
            if (og_rects[r].pattern == 1) {
              pattern |= 0xf;
            } else if (og_rects[r].pattern == 2) { // even row set
              pattern |= 0x3;
            } else if (og_rects[r].pattern == 3) {
              pattern |= 0x5;
            } else {
              pattern |= 0x9;
            }
          }
        }
      }
      // Now bits are set
      printf("pattern: %x, x: %d, y: %d, w: %d, h: %d\n", pattern, xs[i], ys[j],
             xs[i + 1] - xs[i], ys[j + 1] - ys[j]);
    }
  }

  return 0;
}
