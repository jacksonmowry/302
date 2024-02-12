#include "../include/city_map.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

double dist(Intersection *source, Intersection *dest) {
  double x = abs(dest->x - source->x);
  double y = abs(dest->y - source->y);
  return sqrt(x * x + y * y);
}

double City_Map::Dijkstra(int avg_best_worst) {
  bfsq.emplace(0, first);
  first->bfsq_ptr = bfsq.begin();
  while (!bfsq.empty()) {
    pair<double, Intersection *> node = *bfsq.begin();
    bfsq.erase(bfsq.begin());
    node.second->bfsq_ptr = bfsq.end();
    for (Road_Segment *rs : node.second->adj) {
      double dist = node.first + (rs->distance / 30 * 3600);
      if (avg_best_worst == 'W') {
        if (rs->type == STREET) {
          dist += rs->to->green[1];
        } else {
          dist += rs->to->green[0];
        }

      } else if (avg_best_worst == 'A') {
        if (rs->type == STREET) {
          double other = rs->to->green[1];
          double ours = rs->to->green[0];
          dist += (other * other) / (2 * (other + ours));
        } else {
          double other = rs->to->green[0];
          double ours = rs->to->green[1];
          dist += (other * other) / (2 * (other + ours));
        }
      }

      if (rs->to->best_time == -1 || rs->to->best_time > dist) {
        if (rs->to->bfsq_ptr != bfsq.end()) {
          pair<double, Intersection *> inner_node = *rs->to->bfsq_ptr;
          bfsq.erase(rs->to->bfsq_ptr);
          rs->to->bfsq_ptr = bfsq.end();
          inner_node.second->best_time = dist;
          inner_node.second->backedge = rs;
          inner_node.second->bfsq_ptr = bfsq.emplace(dist, inner_node.second);
        } else {
          rs->to->best_time = dist;
          rs->to->backedge = rs;
          rs->to->bfsq_ptr = bfsq.emplace(dist, rs->to);
        }
      }
    }
  }

  double dist = 0;
  Intersection *i = last;
  while (i != first) {
    path.push_back(i->backedge);
    dist += i->backedge->distance;
    i = i->backedge->from;
  }
  reverse(path.begin(), path.end());

  return dist;
}

City_Map::City_Map() {
  Intersection final_node{0, 0, 0, 0, {0, 0}, {}, 0, {}, {}};

  Intersection i;
  while (cin >> i.street >> i.avenue >> i.x >> i.y >> i.green[0] >>
         i.green[1]) {
    i.best_time = -1;
    i.backedge = nullptr;
    i.bfsq_ptr = bfsq.end();
    Intersection *ip = new Intersection;
    *ip = i;
    all.push_back(ip);
    if (i.street > final_node.street || i.avenue > final_node.avenue) {
      final_node = i;
      last = ip;
    }
    if (i.street == 0 && i.avenue == 0) {
      first = ip;
    }
  }

  vector<vector<Intersection *>> tmp(
      final_node.street + 1,
      vector<Intersection *>(final_node.avenue + 1, nullptr));

  for (auto inter : all) {
    tmp[inter->street][inter->avenue] = inter;
  }

  for (Intersection *inter : all) {
    // Up
    if (inter->street < final_node.street &&
        (inter->avenue % 2 == 0 || inter->avenue % 5 == 0 ||
         inter->avenue == final_node.avenue) &&
        tmp[inter->street + 1][inter->avenue]) {
      Intersection *dest = tmp[inter->street + 1][inter->avenue];
      Road_Segment *rs = new Road_Segment;
      *rs = {AVENUE, inter->avenue, dist(inter, dest), inter,
             tmp[inter->street + 1][inter->avenue]};
      inter->adj.push_back(rs);
    }
    // Left
    if (inter->avenue < final_node.avenue &&
        (inter->street % 2 == 0 || inter->street % 5 == 0) &&
        tmp[inter->street][inter->avenue + 1]) {
      Intersection *dest = tmp[inter->street][inter->avenue + 1];
      Road_Segment *rs = new Road_Segment;
      *rs = {STREET, inter->street, dist(inter, dest), inter,
             tmp[inter->street][inter->avenue + 1]};
      inter->adj.push_back(rs);
    }
    // Down
    if (inter->street > 0 &&
        (inter->avenue % 2 != 0 || inter->avenue % 5 == 0 ||
         inter->avenue == final_node.avenue) &&
        tmp[inter->street - 1][inter->avenue]) {
      Intersection *dest = tmp[inter->street - 1][inter->avenue];
      Road_Segment *rs = new Road_Segment;
      *rs = {AVENUE, inter->avenue, dist(inter, dest), inter,
             tmp[inter->street - 1][inter->avenue]};
      inter->adj.push_back(rs);
    }
    // Right
    if (inter->avenue > 0 &&
        (inter->street % 2 != 0 || inter->street % 5 == 0) &&
        tmp[inter->street][inter->avenue - 1]) {
      Intersection *dest = tmp[inter->street][inter->avenue - 1];
      Road_Segment *rs = new Road_Segment;
      *rs = {STREET, inter->street, dist(inter, dest), inter,
             tmp[inter->street][inter->avenue - 1]};
      inter->adj.push_back(rs);
    }
  }
}
