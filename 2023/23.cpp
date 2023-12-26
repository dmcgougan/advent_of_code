/*
 * Problem 23, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

static vector<string> grid;
static int rows;
static int cols;

static vector<string> visited;
static int depth = 0;
static int max_depth = 0;

static void solve_part1(int r, int c)
{
    if (r == rows - 1) {
        max_depth = max(max_depth, depth);
        return;
    }
    ++depth;
    visited[r][c] = 1;
    if (!visited[r + 1][c] && grid[r + 1][c] != '#') {
        solve_part1(r + 1, c);
    }
    if (!visited[r][c + 1] && grid[r][c + 1] != '#') {
        solve_part1(r, c + 1);
    }
    if (r - 1 >= 0 && !visited[r - 1][c]) {
        char ch = grid[r - 1][c];
        if (ch != '#' && ch != 'v') {
            solve_part1(r - 1, c);
        }
    }
    if (!visited[r][c - 1]) {
        char ch = grid[r][c - 1];
        if (ch != '#' && ch != '>') {
            solve_part1(r, c - 1);
        }
    }
    visited[r][c] = 0;
    --depth;
}

static vector<vector<array<int, 2>>> adj;
static vector<int> p2_visited;
static int goal_node;

static void solve_part2(int n)
{
    if (n == goal_node) {
        max_depth = max(max_depth, depth);
        return;
    }
    p2_visited[n] = 1;
    for (auto [to, len] : adj[n]) {
        if (!p2_visited[to]) {
            depth += len;
            solve_part2(to);
            depth -= len;
        }
    }
    p2_visited[n] = 0;
}

int main()
{
    // Parse input data
    string line;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    rows = grid.size();
    cols = grid[0].size();
    visited = vector(rows, string(cols, '\0'));

    // Find all cells where we have more than one choice of where to go
    static const vector<array<int, 2>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    set<array<int, 2>> nodes;
    nodes.insert({0, 1});
    nodes.insert({rows - 1, cols - 2});
    for (int r = 1; r < rows - 1; ++r) {
        for (int c = 1; c < cols - 1; ++c) {
            if (grid[r][c] == '#') continue;
            int d = 0;
            for (auto [dr, dc] : dirs) {
                if (grid[r + dr][c + dc] != '#') {
                    ++d;
                }
            }
            if (d >= 3) {
                nodes.insert({r, c});
            }
        }
    }

    // Build adjacency list
    map<array<int, 2>, int> node_ix;
    int i = 0;
    for (auto [r, c] : nodes) {
        node_ix[{r, c}] = i++;
    }
    goal_node = node_ix.at({rows - 1, cols - 2});
    for (auto [r, c] : nodes) {
        vector<array<int, 2>> to_list;
        vector<array<int, 3>> s;
        set<array<int, 2>> visited;
        s.push_back({r, c, 1});
        while (!s.empty()) {
            auto [r, c, d] = s.back();
            s.pop_back();
            if (visited.count({r, c})) continue;
            visited.insert({r, c});
            for (auto [dr, dc] : dirs) {
                int nr = r + dr;
                int nc = c + dc;
                if (nr < 0 || nr >= rows) continue;
                if (nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] == '#') continue;
                if (nodes.count({nr, nc})) {
                    to_list.push_back({node_ix.at({nr, nc}), d});
                    continue;
                }
                s.push_back({nr, nc, d + 1});
            }
        }
        adj.push_back(to_list);
    }
    p2_visited = vector<int>(adj.size());

    // Part 1
    max_depth = 0;
    solve_part1(0, 1);
    int part1 = max_depth;

    // Part 2
    max_depth = 0;
    solve_part2(0);
    int part2 = max_depth;

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
