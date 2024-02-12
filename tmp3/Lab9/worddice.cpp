#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

using namespace std;

struct Node;

struct Edge {
    Node* from;
    Node* to;
    Edge* reverse;
    int original;
    int residual;
};

enum NodeType {
    SOURCE,
    DIE,
    LETTER,
    SINK,
};

struct Node {
    NodeType type;
    bool letters[256];
    int visted;
    vector<Edge*> adj;
    Edge* backedge;
    bool seen;
};

vector<Edge*> pathfind(vector<Node*>& graph, Node* source, Node* sink) {
    for (auto n : graph) {
        n->backedge = nullptr;
        n->visted = false;
    }
    vector<Edge*> path;
    multimap<int, Node*> q;
    q.emplace(1, source);
    while (!q.empty()) {
        pair<int, Node*> p = *q.begin();
        p.second->visted = true;
        q.erase(q.begin());
        if (p.second == sink) {
            // Found a route to the end
            Node* cur = p.second;
            while (cur != source) {
                path.push_back(cur->backedge);
                cur = cur->backedge->from;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        for (Edge* e : p.second->adj) {
            if (e->original == 1 &&
                !e->to->visted) { // only follow paths orig=1
                e->to->backedge = e;
                q.emplace(e->residual, e->to);
            }
        }
    }

    path.clear();
    return path;
}

int main(int argc, char** argv) {
    ifstream words(argv[2]);
    string input;
    while (words >> input) {
        string desired_word = input;
        vector<Node*> graph;
        graph.push_back(new Node{SOURCE, {0}, 0, {}, nullptr});

        ifstream dice(argv[1]);

        while (dice >> input) {
            graph.push_back(new Node{DIE, {0}, 0, {}, nullptr});
            // Reverse Dice -> Source
            graph.back()->adj.push_back(
                new Edge{graph.back(), graph[0], nullptr, 0, 1});
            // Forward Source -> Dice
            graph[0]->adj.push_back(new Edge{
                graph[0], graph.back(), graph.back()->adj.back(), 1, 0});

            // Update reverse edge "reverse"
            graph.back()->adj.back()->reverse = graph[0]->adj.back();

            for (auto c : input) {
                graph.back()->letters[c] = true;
            }
        }

        size_t first_letter = graph.size();
        for (auto c : desired_word) {
            graph.push_back(new Node{LETTER, {0}, 0, {}, nullptr});
            graph.back()->letters[c] = true;
            for (size_t i = 1; i < graph.size() && graph[i]->type == DIE;
                 i++) {
                if (graph[i]->letters[c]) {
                    // reverse edge Letter -> Dice
                    graph.back()->adj.push_back(
                        new Edge{graph.back(), graph[i], nullptr, 0, 1});
                    // actual edge Dice -> Letter
                    graph[i]->adj.push_back(
                        new Edge{graph[i], graph.back(),
                                 graph.back()->adj.back(), 1, 0});
                    // Update reverse edge "reverse"
                    graph.back()->adj.back()->reverse =
                        graph[i]->adj.back();
                }
            }
        }

        graph.push_back(new Node{SINK, {0}, 0, {}, nullptr});

        // Make letters point to the sink
        for (size_t i = first_letter; i < graph.size() - 1; i++) {
            // Reverse Sink -> Letter
            graph.back()->adj.push_back(
                new Edge{graph.back(), graph[i], nullptr, 1, 0});
            // Forward Letter -> Sink
            graph[i]->adj.push_back(new Edge{
                graph[i], graph.back(), graph.back()->adj.back(), 1, 0});
            // Update reverse edge "reverse"
            graph.back()->adj.back()->reverse = graph[i]->adj.back();
        }

        Node* source = graph[0];
        Node* sink = graph.back();

        int flow = 0;
        vector<Edge*> path;
        while ((path = pathfind(graph, source, sink)).size()) {
            flow++;
            for (auto e : path) {
                // original=0 & residual=1 on a normal edge
                // original=1 & residual=0  on a reverse edge
                e->original = 0;
                e->residual = 1;
                e->reverse->original = 1;
                e->reverse->residual = 0;
            }
        }
        int count = 0;
        vector<int> die_nums;
        for (Edge* e : sink->adj) {
            if (e->reverse->residual == 1) {
                Node* die;
                for (Edge* b : e->to->adj) {
                    if (b->original == 1) {
                        die = b->to;
                    }
                }
                for (size_t i = 0; i < graph.size(); i++) {
                    if (graph[i] == die) {
                        die_nums.push_back(i - 1);
                    }
                }
                count++;
            }
        }
        if (count == desired_word.size()) {
            for (size_t i = 0; i < die_nums.size() - 1; i++) {
                printf("%d,", die_nums[i]);
            }
            printf("%d: ", die_nums.back());
            printf("%s\n", desired_word.c_str());
        } else {
            printf("Cannot spell %s\n", desired_word.c_str());
        }

        for (Node* n : graph) {
            for (Edge* e : n->adj) {
                delete e;
            }
            delete n;
        }
    }
}
