#include "cs302-midi.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>
using namespace std;

void CS302_Midi::el_to_nd() {
  nd = new NDMap;
  int time = 0;
  vector<ND *> notes(128, nullptr);
  ND *damper = nullptr;
  for (auto i : *el) {
    time += i->time;
    switch (i->key) {
    case 'O':
      notes[i->v1] = new ND;
      *notes[i->v1] = {'N', i->v1, i->v2, (double)time / 480};
      break;
    case 'F':
      if (notes[i->v1] == nullptr) {
        fprintf(stderr, "Attempt to end a note which has not yet started\n");
        exit(69);
      }
      (*notes[i->v1]).stop = (double)time / 480;
      nd->emplace(notes[i->v1]->start, notes[i->v1]);
      notes[i->v1] = nullptr;
      break;
    case 'D':
      if (i->v1 == 0 && !damper) {
        fprintf(
            stderr,
            "Attempt to lift the damper, which has not yet been depressed\n");
        exit(69);
      } else if (i->v1 == 1 && !damper) {
        damper = new ND;
        *damper = {'D', 0, 0, (double)time / 480};
      } else if (i->v1 == 0 && damper) {
        damper->stop = (double)time / 480;
        nd->emplace(damper->start, damper);
        damper = nullptr;
      } else {
        fprintf(stderr, "Error damper\n");
        exit(420);
      }
      break;
    default:
      exit(42);
    }
  }
}

void CS302_Midi::nd_to_el() {
  multimap<int, Event> event_stream;
  set<int> absolute_times;
  NDMap::iterator it = this->nd->begin();
  for (; it != this->nd->end(); it++) {
    Event e_start;
    Event e_end;
    if (it->second->key == 'N') {
      e_start = {'O', 6969, it->second->pitch, it->second->volume};
      e_end = {'F', 6969, it->second->pitch, 0};
    } else {
      e_start = {'D', 6969, 1, 0}; // last 0 is ignored
      e_end = {'D', 6969, 0, 0};
    }
    int start = (int)rint(it->second->start * 480);
    int stop = (int)rint(it->second->stop * 480);
    if (start == stop) {
      continue;
    }
    event_stream.emplace(start, e_start);
    event_stream.emplace(stop, e_end);
    absolute_times.insert(start);
    absolute_times.insert(stop);
  }
  el = new EventList;
  int prev = 0;
  set<int>::iterator sit = absolute_times.begin();
  for (; sit != absolute_times.end(); sit++) {
    auto time_range = event_stream.equal_range(*sit);
    auto i = time_range.first;
    int offset = i->first - prev;
    Event e = i->second;
    e.time = offset;
    prev = i->first;
    Event *new_e = new Event;
    *new_e = e;
    el->push_back(new_e);
    i++;
    for (; i != time_range.second; i++) {
      Event e = i->second;
      e.time = 0;
      Event *new_e = new Event;
      *new_e = e;
      el->push_back(new_e);
    }
  }
}
