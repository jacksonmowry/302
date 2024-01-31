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

double pythag(double x, double y) { return sqrt(pow(x, 2) + pow(y, 2)); }

struct RadioRange {
  // Returns a value within 1e-6 of the real result
  double RadiusProbability(vector<int> X, vector<int> Y, vector<int> R, int Z) {
    // `X` Contains x coords of cities
    // `Y` Contains y coords of cities
    // `R` Contains radius of cities
    vector<double> close;
    vector<double> far;
    for (int i = 0; i < X.size(); i++) {
      close.push_back(pythag(X[i], Y[i]) - R[i]);
      far.push_back(pythag(X[i], Y[i]) + R[i]);
    }
    for (auto i : close) {
      cout << close[i] << ' ';
    }
    cout << endl;
    for (auto i : far) {
      cout << far[i] << ' ';
    }
    cout << endl;
    return 0.0;
  };
};

int main() {
  vector<int> x = {10, -12, 0, 0};
  vector<int> y = {0, 0, 2, -18};
  vector<int> r = {5, 5, 1, 1};
  RadioRange rr = RadioRange();
  rr.RadiusProbability(x, y, r, 1);
}
