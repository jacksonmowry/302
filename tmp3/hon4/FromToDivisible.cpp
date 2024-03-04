#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

class FromToDivisible {
public:
  int shortest(int N, int S, int T, vector<int> a, vector<int> b);
  uint64_t gcd(uint64_t x, uint64_t y) {
    if (x < y) {
      swap(x, y);
    }
    assert(x >= y);
    int a = x % y;
    if (a == 0) {
      return y;
    }
    return gcd(y, a);
  }
};

struct Node {
  vector<uint64_t> edges;
  int dist;
  int pos;
};

int FromToDivisible::shortest(int N, int S, int T, vector<int> a,
                              vector<int> b) {
  vector<Node> adj(1000);
  for (size_t i = 0; i < adj.size(); i++) {
    adj[i] = {{}, -1, (int)i};
  }

  Node start = {{}, -1, S};
  Node sink = {{}, -1, T};

  for (size_t i = 0; i < a.size(); i++) {
    if (S % a[i] == 0) {
      start.edges.push_back(i);
    }
    if (T % b[i] == 0) {
      adj[i].edges.push_back(T);
    }
  }

  for (size_t i = 0; i < b.size(); i++) {
    for (size_t j = 0; j < a.size(); j++) {
      if (j == i) {
        continue;
      }
      int g = gcd(b[i], a[j]);
      if (g <= 0) {
        continue;
      }
      uint64_t new_gcd = ((uint64_t)b[i] * (uint64_t)a[j]) / g;
      if (new_gcd >= 1 && new_gcd <= N) {
        adj[i].edges.push_back(j);
      }
    }
  }

  queue<Node *> q;
  start.dist = 0;
  q.push(&start);
  while (!q.empty()) {
    Node *n = q.front();
    q.pop();
    if (n->pos == T) {
      return n->dist - 1;
    }
    for (uint64_t edge : n->edges) {
      if (edge > 1000) {
        sink.dist = n->dist + 1;
        q.push(&sink);
        return n->dist;
        continue;
      }
      if (adj[edge].dist != -1) {
        continue;
      }
      adj[edge].dist = n->dist + 1;
      q.push(&adj[edge]);
    }
  }

  return -1;
}
