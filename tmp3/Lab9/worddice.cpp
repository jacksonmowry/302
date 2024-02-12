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
    int flow;
    bool tmp;
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
    multimap<int, Node*>::iterator qit;
};

vector<Edge*> pathfind(vector<Node*>& graph, Node* source, Node* sink) {
    vector<Edge*> path;
    multimap<int, Node*> q;
    int flow;
    source->qit = q.emplace(INT32_MAX, source);
    while (!q.empty()) {
        pair<int, Node*> p = *q.begin();
        q.erase(q.begin());
        p.second->qit = q.end();
        if (p.second == sink) {
            // Found a route to the end
            Node* cur = p.second;
            while (cur != source) {
                // printf("type %d\n", cur->type);
                path.push_back(cur->backedge);
                cur = cur->backedge->from;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        for (Edge* e : p.second->adj) {
            if (e->to != source &&
                e->original > e->flow) { // if pred[e.t] == null
                // add to pred
                e->to->backedge = e;
                e->to->qit = q.emplace(e->residual, e->to);
            }
        }
    }

    return path;
}

int main(int argc, char** argv) {
    vector<Node*> graph;
    graph.push_back(new Node{SOURCE, {0}, 0, {}, nullptr});

    ifstream dice(argv[1]);
    ifstream words(argv[2]);

    string input;
    while (dice >> input) {
        graph.push_back(new Node{DIE, {0}, 0, {}, nullptr});
        graph.back()->adj.push_back(
            new Edge{graph.back(), graph[0], nullptr, 0, 1, 1, false});
        graph[0]->adj.push_back(new Edge{graph[0], graph.back(),
                                         graph.back()->adj.back(), 1, 0, 0,
                                         false});
        for (auto c : input) {
            graph.back()->letters[c] = true;
        }
    }

    while (words >> input) {
        // need to reset all edges back to default values first
        for (Node* n : graph) {
            n->backedge = nullptr;
            size_t prev = n->adj.size();
            for (ssize_t i = n->adj.size() - 1; i >= 0; i -= 1) {
                if (n->adj[i]->tmp) {
                    n->adj.erase(n->adj.begin() + i);
                }
            }
            assert(prev >= n->adj.size());
            for (size_t i = 0; i < n->adj.size() - 1; i += 2) {
                if (n->adj[i]->residual == 1) {
                    // reverse edge
                    n->adj[i]->flow = 1;
                } else {

                    // forward edge
                    n->adj[i + 1]->flow = 0;
                }
            }
        }

        vector<Node*> copy = graph;
        size_t first_letter = copy.size();
        for (auto c : input) {
            copy.push_back(new Node{LETTER, {0}, 0, {}, nullptr});
            copy.back()->letters[c] = true;
            for (size_t i = 1; i < copy.size() && copy[i]->type == DIE;
                 i++) {
                if (copy[i]->letters[c]) {
                    copy.back()->adj.push_back(
                        new Edge{copy.back(), copy[i], nullptr, 0, 1, 1,
                                 true}); // reverse edge
                    copy[i]->adj.push_back(
                        new Edge{copy[i],
                                 copy.back(), // actual edge
                                 copy.back()->adj.back(), 1, 0, 0, true});
                    copy.back()->adj.back()->reverse = copy[i]->adj.back();
                }
            }
        }

        copy.push_back(new Node{SINK, {0}, 0, {}, nullptr});

        // Make letters point to the sink
        for (size_t i = first_letter; i < copy.size() - 1; i++) {
            copy.back()->adj.push_back(
                new Edge{copy.back(), copy[i], nullptr, 1, 0, 1, true});
            copy[i]->adj.push_back(new Edge{copy[i], copy.back(),
                                            copy.back()->adj.back(), 1, 0,
                                            0, true});
            copy.back()->adj.back()->reverse = copy[i]->adj.back();
        }

        Node* source = copy[0];
        Node* sink = copy.back();

        int flow = 0;
        vector<Edge*> path;
        while ((path = pathfind(copy, source, sink)).size()) {
            flow++;
            for (auto e : path) {
                e->flow = 1;
                e->reverse->flow = 0;
                printf("node type: %d\n", e->from->type);
                for (int i = 65; i < 91; i++) {
                    printf("%c ", (e->from->letters[i]) ? i : ' ');
                }
                printf("\n");
            }
        }
        printf("flow: %d\n", flow);
        if (flow == input.size()) {
            printf("can spell %s\n", input.c_str());
        } else {
            printf("cannot spell %s\n", input.c_str());
        }
    }
}
