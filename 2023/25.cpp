/*
 * Problem 25, Advent of Code 2023
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static vector<vector<tuple<int, int, bool>>> adj;
static vector<array<int, 5>> edge_list;
static int nodes;
static int edges;

static int bfs(int node)
{
    int ret = 0;
    vector<int> visited(nodes, 0);
    deque<array<int, 3>> q;
    q.push_back({node, -1, 0});
    while (!q.empty()) {
        auto [n, from, adj_ix] = q.front();
        q.pop_front();
        if (visited[n]) continue;
        visited[n] = 1;
        if (from >= 0) get<1>(adj[from][adj_ix])++;
        ++ret;
        int i = 0;
        for (auto [to, use_count, valid] : adj[n]) {
            if (valid && !visited[to]) q.push_back({to, n, i});
            i++;
        }
    }
    return ret;
}

static inline void remove_edge(int e)
{
    auto [use_count, from, to_ix, to, from_ix] = edge_list[e];
    get<2>(adj[from][to_ix]) = false;
    get<2>(adj[to][from_ix]) = false;
}

static inline void add_edge(int e)
{
    auto [use_count, from, to_ix, to, from_ix] = edge_list[e];
    get<2>(adj[from][to_ix]) = true;
    get<2>(adj[to][from_ix]) = true;
}

static int solve()
{
    for (int i = 0; i < edges; ++i) {
        remove_edge(i);
        for (int j = i + 1; j < edges; ++j) {
            remove_edge(j);
            for (int k = j + 1; k < edges; ++k) {
                remove_edge(k);
                int c = bfs(0);
                if (c < nodes) {
                    return c * (nodes - c);
                }
                add_edge(k);
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
        node_map[s] = nodes++;
    }
    adj = vector<vector<tuple<int, int, bool>>>(nodes);
    for (const auto& [from, to_list] : adj_str) {
        int from_ix = node_map.at(from);
        for (const auto& to : to_list) {
            int to_ix = node_map.at(to);
            adj[from_ix].push_back({to_ix, 0, true});
        }
    }

    // Create a separate list of undirected edges
    set<array<int, 2>> edge_set;
    for (int from = 0; from < nodes; ++from) {
        for (auto [to, use_count, valid] : adj[from]) {
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
            if (get<0>(adj[from][i]) == to) {
                from_adj_ix = i;
                break;
            }
        }
        for (size_t i = 0; i < adj[to].size(); ++i) {
            if (get<0>(adj[to][i]) == from) {
                to_adj_ix = i;
                break;
            }
        }
        assert(from_adj_ix >= 0);
        assert(to_adj_ix >= 0);
        edge_list.push_back({0, from, from_adj_ix, to, to_adj_ix});
    }

    // Walk the whole graph starting from all nodes to get usage counts for all edges
    for (int n = 0; n < nodes; ++n) {
        bfs(n);
    }
    for (int n = 0; n < nodes; ++n) {
        for (auto [adj_to, adj_use_count, valid] : adj[n]) {
            for (auto& [use_count, from, from_adj_ix, to, to_adj_ix] : edge_list) {
                if ((from == n && to == adj_to) || (from == adj_to && to == n)) {
                    use_count += adj_use_count;
                    break;
                }
            }
        }
    }

    // Sort the edge list in descending order of use count
    // The most used edges are likely the ones we need to cut
    sort(edge_list.rbegin(), edge_list.rend());

    // Brute force solver starting by removing the most used edges
    int part1 = solve();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
