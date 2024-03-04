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

struct Node {
  vector<int> edges;
  int dist;
  int pos;
};

class FromToDivisible {
public:
  int shortest(int N, int S, int T, vector<int> a, vector<int> b);
  int gcd(int x, int y) {
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

int FromToDivisible::shortest(int N, int S, int T, vector<int> a,
                              vector<int> b) {
  vector<Node> adj_list(1000, {{}, -1});
  for (size_t i = 0; i < adj_list.size(); i++) {
    adj_list[i].pos = i;
  }
  // handle S and T
  Node start = {{}, -1, S};
  Node sink = {{}, -1, T};
  for (size_t i = 0; i < a.size(); i++) {
    if (S % a[i] == 0) {
      start.edges.push_back(i);
    }
    if (T % b[i] == 0) {
      printf("SINK: %d is a multiple of %d\n", T, b[i]);
      adj_list[i].edges.push_back(T);
    }
  }
  for (size_t i = 0; i < b.size(); i++) {
    if (i == S || i == T) {
      continue;
    }
    for (size_t j = 0; j < a.size(); j++) {
      if (j == i) {
        continue;
      }
      int lcm = gcd(b[i], a[j]);
      int new_gcd = 0;
      if (lcm > 0) {
        new_gcd = (b[i] * a[j]) / lcm;
      }
      if (new_gcd > 1 && new_gcd <= N) {
        adj_list[i].edges.push_back(j);
      }
    }
  }

  // printf("node start | ");
  // for (auto a : start.edges) {
  //   printf("%d ", a);
  // }
  // printf("\n");
  // for (size_t i = 0; i < adj_list.size(); i++) {
  //   printf("node %zu | ", i);
  //   for (auto a : adj_list[i].edges) {
  //     printf("%d ", a);
  //   }
  //   printf("\n");
  // }

  // BFS
  queue<Node *> q;
  start.dist = 0;
  q.push(&start);
  while (!q.empty()) {
    Node *n = q.front();
    q.pop();
    if (n->pos == T) {
      return n->dist - 1;
    }
    for (auto edge : n->edges) {
      if (edge == T) {
        sink.dist = n->dist + 1;
        q.push(&sink);
        return n->dist;
        continue;
      }
      if (adj_list[edge].dist != -1) {
        continue;
      }
      adj_list[edge].dist = n->dist + 1;
      q.push(&adj_list[edge]);
    }
  }
  return -1;
}
