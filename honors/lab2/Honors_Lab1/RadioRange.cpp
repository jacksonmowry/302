// Jackson Mowry
// Feb 01 2024
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct RadioRange {
  // Returns a value within 1e-6 of the real result
  double pythag(double x, double y) { return sqrt(pow(x, 2) + pow(y, 2)); }
  double RadiusProbability(vector<int> X, vector<int> Y, vector<int> R, int Z) {
    vector<pair<double, double>> coords;
    for (int i = 0; i < X.size(); i++) {
      // Calculate distances from close edge and far edge
      // Cities that have their "close" edge overlapping the origin
      // can be set to 0.0
      double close = max(pythag(X[i], Y[i]) - R[i], 0.0);
      double far = (pythag(X[i], Y[i]) + R[i]);
      coords.push_back(make_pair(close, far));
    }
    // sort
    sort(coords.begin(), coords.end());
    vector<pair<double, double>> collapsed;
    collapsed.push_back(coords[0]);
    for (int i = 1; i < coords.size(); i++) {
      if (coords[i].first >= (double)Z) {
        // We have no need to record values for cities past max
        // range of the radio station
        break;
      }
      pair<double, double> prev = coords[i - 1];
      if (coords[i].first <= prev.second) {
        // If our current range begins before the previous one ends
        // we merge ourselves into the previous range
        collapsed.back().second = max(prev.second, coords[i].second);
      } else {
        // Otherwise we do not overlap, and we can add to the range
        collapsed.push_back(coords[i]);
      }
    }
    if (collapsed[0].first >= Z) {
      // The closes towns "close" edge is further than our max range
      return 1;
    }
    double space = collapsed[0].first;
    for (int i = 1; i < collapsed.size(); i++) {
      // Add the spaces between the collapsed ranges
      double add = max(collapsed[i].first - collapsed[i - 1].second, 0.0);
      space += add;
      if (add == 0.0) {
        collapsed[i].second = collapsed[i - 1].second;
      }
    }
    if (collapsed.back().second < Z) {
      // There was a gap between the furthest town and our max range
      space += Z - collapsed.back().second;
    }
    return space / (double)Z;
  };
};
