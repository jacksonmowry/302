// Jackson Mowry
// Wed Jan 24 2024
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Song {
  string title;
  int time;
  int track;
};

struct Album {
  map<int, Song> songs;
  string name;
  int time;
};

struct Artist {
  map<string, Album> albums;
  string name;
  int time;
  int nsongs;
};

typedef map<string, Artist>::iterator art_it;
typedef map<string, Album>::iterator alb_it;
typedef map<int, Song>::iterator song_it;

// mmss_to_sec takes a string in the form "mm:ss" and returns
// it's total time in seconds
int mmss_to_sec(string mmss) {
  size_t pos = mmss.find(":");
  mmss[pos] = ' ';
  stringstream buf(mmss);
  int minutes;
  int seconds;
  buf >> minutes >> seconds;
  return (minutes * 60) + seconds;
}

// sec_to_mmss takes a total time in seconds, and returns
// a string formatted as "mm:ss" to represent the time
string sec_to_mmss(int seconds) {
  stringstream buf;
  buf << seconds / 60;
  buf << ':';
  buf << setw(2) << setfill('0') << seconds % 60;
  return buf.str();
}

// replace_underscores modifies a string in place to
// replace all underscores with spaces
void replace_underscores(string &s) {
  transform(s.cbegin(), s.cend(),
            s.begin(), // write to the same location
            [](char c) {
              if (c == '_') {
                return ' ';
              } else {
                return c;
              }
            });
}

void artist_print(Artist a) {
  cout << a.name << ": " << a.nsongs << ", " << sec_to_mmss(a.time) << endl;
}

void album_print(Album a) {
  cout << setw(8) << ' ' << a.name << ": " << a.songs.size() << ", "
       << sec_to_mmss(a.time) << endl;
}

void song_print(Song s) {
  cout << setw(16) << ' ' << s.track << ". " << s.title << ": "
       << sec_to_mmss(s.time) << endl;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "usage: lib_info file" << endl;
    exit(1);
  }

  ifstream fin(argv[1]);
  if (!fin) {
    cerr << "Couldn't open " << argv[1] << endl;
    exit(1);
  }

  string title;
  string time;
  string artist;
  string album;
  string genre;
  int track; // Track numbers can be read in directly as ints

  map<string, Artist> artist_map;

  while (fin >> title >> time >> artist >> album >> genre >> track) {
    int seconds = mmss_to_sec(time);
    replace_underscores(title);
    replace_underscores(artist);
    replace_underscores(album);
    replace_underscores(genre);

    Song s = {title, seconds, track};

    art_it art_it;
    alb_it alb_it;

    if ((art_it = artist_map.find(artist)) != artist_map.end()) {
      // Artist exists, update time and song count
      Artist *curr_artist = &(*art_it).second;
      curr_artist->time += seconds;
      curr_artist->nsongs += 1;
      // (*art_it).second.time += seconds;
      // (*art_it).second.nsongs += 1;
      if ((alb_it = curr_artist->albums.find(album)) !=
          curr_artist->albums.end()) {
        // Album exists, update time and track count
        Album *curr_album = &(*alb_it).second;
        curr_album->songs[track] = s;
        curr_album->time += seconds;
      } else {
        // Album does not exist, create it and store song
        map<int, Song> songs_map;
        Album a = {songs_map, album, seconds};
        a.songs[track] = s;
        curr_artist->albums[album] = a;
      }
    } else {
      // artist doesn't exist, create artist, create album and store song
      Album a = {{}, album, seconds};
      a.songs[track] = s;
      map<string, Album> album_map;
      album_map[album] = a;
      artist_map[artist] = {album_map, artist, seconds, 1};
    }
  }
  fin.close();

  for (art_it it = artist_map.begin(); it != artist_map.end(); ++it) {
    // Iterating through artists
    artist_print((*it).second);
    for (alb_it albs = (*it).second.albums.begin();
         albs != (*it).second.albums.end(); ++albs) {
      // Iterating through albums
      album_print((*albs).second);
      for (song_it songs = (*albs).second.songs.begin();
           songs != (*albs).second.songs.end(); ++songs) {
        // Iterating through songs
        song_print((*songs).second);
      }
    }
  }
}
