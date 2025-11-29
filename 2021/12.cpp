/*
 * Problem 12, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const std::string& str, const std::string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.push_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.push_back(move(s));
    }
    return result;
}

// Check if we are allowed to visit the specified node
static bool can_visit(const string& node,
                      const unordered_map<string, int>& visited,
                      bool allow_twice)
{
    if (isupper(node[0])) return true;
    if (node == "start") return false;
    auto iter = visited.find(node);
    if (iter == visited.end() || iter->second == 0) return true;
    if (!allow_twice) return false;
    for (const auto& [n, c] : visited) {
        if (c > 1 && islower(n[0])) return false;
    }
    return true;
}

// Count the number of paths from start to end
static int count_paths(const string& node,
                       const unordered_map<string, vector<string>>& adj,
                       unordered_map<string, int>& visited,
                       bool allow_twice)
{
    if (node == "end") return 1;

    int count = 0;
    for (const string& v : adj.at(node)) {
        if (can_visit(v, visited, allow_twice)) {
            ++visited[v];
            count += count_paths(v, adj, visited, allow_twice);
            --visited[v];
        }
    }

    return count;
}

int main()
{
    unordered_map<string, vector<string>> adj;
    string line;
    while (getline(cin, line)) {
        vector<string> nodes = split(line, "-");
        assert(nodes.size() == 2);
        adj[nodes[0]].push_back(nodes[1]);
        adj[nodes[1]].push_back(nodes[0]);
    }

    unordered_map<string, int> visited;
    int part1 = count_paths("start", adj, visited, false);
    int part2 = count_paths("start", adj, visited, true);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
