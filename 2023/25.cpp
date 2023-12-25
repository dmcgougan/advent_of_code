/*
 * Problem 25, Advent of Code 2023
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

//
// This code solves the problem but it's slow
// Runs in about 80 seconds on my computer
//

static vector<vector<pair<int, bool>>> adj;
static vector<array<int, 4>> edge_list;
static vector<string> node_name;
static int nodes;
static int edges;

static vector<int> ap_visited;
static vector<int> ap_disc;
static vector<int> ap_low;
static int ap_time;

// Check if the graph has at least one articulation point
// Adapted from https://www.geeksforgeeks.org/articulation-points-or-cut-vertices-in-a-graph/
static bool ap_rec(int u, int parent = -1)
{
    // Count of children in DFS Tree
    int children = 0;

    // Mark the current node as visited
    ap_visited[u] = 1;

    // Initialize discovery time and low value
    ap_disc[u] = ap_low[u] = ++ap_time;

    // Go through all vertices adjacent to this
    for (auto [v, valid] : adj[u]) {
        if (!valid) continue;

        // If v is not visited yet, then make it a child of u
        // in DFS tree and recur for it
        if (!ap_visited[v]) {
            children++;
            if (ap_rec(v, u)) return true;

            // Check if the subtree rooted with v has
            // a connection to one of the ancestors of u
            ap_low[u] = min(ap_low[u], ap_low[v]);

            // If u is not root and low value of one of
            // its child is more than discovery value of u.
            if (parent >= 0 && ap_low[v] >= ap_disc[u]) {
                return true;
            }
        }

        // Update low value of u for parent function calls.
        else if (v != parent)
            ap_low[u] = min(ap_low[u], ap_disc[v]);
    }

    // If u is root of DFS tree and has two or more children.
    if (parent == -1 && children > 1) {
        return true;
    }

    return false;
}

static bool ap(void)
{
    fill(ap_visited.begin(), ap_visited.end(), 0);
    fill(ap_disc.begin(), ap_disc.end(), 0);
    fill(ap_low.begin(), ap_low.end(), 0);
    ap_time = 0;
    return ap_rec(0, -1);
}

static int count(int node)
{
    int ret = 0;
    vector<int> visited(nodes, 0);
    deque<int> q;
    q.push_back(node);
    while (!q.empty()) {
        int n = q.front();
        q.pop_front();
        if (visited[n]) continue;
        visited[n] = 1;
        ++ret;
        for (auto [to, valid] : adj[n]) {
            if (valid && !visited[to]) q.push_back(to);
        }
    }
    return ret;
}

static inline void remove_edge(int e)
{
    auto [from, to_ix, to, from_ix] = edge_list[e];
    adj[from][to_ix].second = false;
    adj[to][from_ix].second = false;
}

static inline void add_edge(int e)
{
    auto [from, to_ix, to, from_ix] = edge_list[e];
    adj[from][to_ix].second = true;
    adj[to][from_ix].second = true;
}

static int solve1()
{
    for (int i = 0; i < edges; ++i) {
        remove_edge(i);
        for (int j = i + 1; j < edges; ++j) {
            remove_edge(j);
            // See if we can find at least one articulation point after removing 2 edges
            // An articulation point is a vertex that connects two otherwise
            // unconnected parts of the graph
            if (ap()) {
                // Yes, we found an articulation point
                // See which edge needs to be removed to disconnect the graph
                for (int k = j + 1; k < edges; ++k) {
                    remove_edge(k);
                    int c = count(0);
                    if (c < nodes) {
                        return c * (nodes - c);
                    }
                    add_edge(k);
                }
            }
            add_edge(j);
        }
        add_edge(i);
    }
    return 0;
}

int main()
{
    // Parse input data
    string line;
    set<string> node_set;
    map<string, set<string>> adj_str;
    while (getline(cin, line)) {
        stringstream ss(line);
        string from;
        ss >> from;
        from = from.substr(0, from.size() - 1);
        string to;
        while (ss >> to) {
            adj_str[from].insert(to);
            adj_str[to].insert(from);
            node_set.insert(to);
        }
        node_set.insert(from);
    }

    // Convert node names to indices to speed up the code
    map<string, int> node_map;
    for (const auto& s : node_set) {
        node_name.push_back(s);
        node_map[s] = nodes++;
    }
    adj = vector<vector<pair<int, bool>>>(nodes);
    for (const auto& [from, to_list] : adj_str) {
        int from_ix = node_map.at(from);
        for (const auto& to : to_list) {
            int to_ix = node_map.at(to);
            adj[from_ix].push_back({to_ix, true});
        }
    }
    set<array<int, 2>> edge_set;
    for (int from = 0; from < nodes; ++from) {
        for (auto [to, valid] : adj[from]) {
            if (!edge_set.count({to, from})) {
                edge_set.insert({from, to});
            }
        }
    }
    edges = edge_set.size();
    for (auto [from, to] : edge_set) {
        int from_adj_ix = -1;
        int to_adj_ix = -1;
        for (size_t i = 0; i < adj[from].size(); ++i) {
            if (adj[from][i].first == to) {
                from_adj_ix = i;
                break;
            }
        }
        for (size_t i = 0; i < adj[to].size(); ++i) {
            if (adj[to][i].first == from) {
                to_adj_ix = i;
                break;
            }
        }
        assert(from_adj_ix >= 0);
        assert(to_adj_ix >= 0);
        edge_list.push_back({from, from_adj_ix, to, to_adj_ix});
    }

    // Prepare data structures for the AP code
    ap_visited = vector(nodes, 0);
    ap_disc = vector(nodes, 0);
    ap_low = vector(nodes, 0);

    // Part 1
    int part1 = solve1();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
