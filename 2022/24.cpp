/*
 * Problem 24, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Blizzards
static ll rows, cols;
static vector<vector<ll>> h_bliz;  // indexed by row
static vector<vector<ll>> v_bliz;  // indexed by col

// Is the given row and column safe at time t?
static bool is_safe(ll row, ll col, ll t)
{
    // Check horizontal blizzards
    ll n, p;
    p = (col + cols - (t % cols)) % cols + 1;
    n = -((col + t) % cols) - 1;
    for (ll b : h_bliz[row]) {
        if (b == p || b == n) return false;
    }
    // Check vertical blizzards
    p = (row + rows - (t % rows)) % rows + 1;
    n = -((row + t) % rows) - 1;
    for (ll b : v_bliz[col]) {
        if (b == p || b == n) return false;
    }
    return true;
}

// DP memoize
static map<ll, ll> cache;
static ll g_min_moves = INT64_MAX;

// Depth-first search going forward
static ll dfs_f(ll row, ll col, ll t)
{
    auto iter = cache.find(t * rows * cols + row * cols + col);
    if (iter != cache.end()) return iter->second;
    if (row == rows - 1 && col == cols - 1) {
        return t + 1;
    }
    ll min_moves = INT64_MAX;
    // If we already found a better minimum return directly
    if (t + 1 >= g_min_moves) {
        return min_moves;
    }
    // Search down and right moves first since they lead to the goal
    if (row + 1 < rows && is_safe(row + 1, col, t + 1)) {
        min_moves = min(min_moves, dfs_f(row + 1, col, t + 1));
    }
    if (col + 1 < cols && row >= 0 && is_safe(row, col + 1, t + 1)) {
        min_moves = min(min_moves, dfs_f(row, col + 1, t + 1));
    }
    if (row - 1 >= 0 && is_safe(row - 1, col, t + 1)) {
        min_moves = min(min_moves, dfs_f(row - 1, col, t + 1));
    }
    if (col - 1 >= 0 && row >= 0 && is_safe(row, col - 1, t + 1)) {
        min_moves = min(min_moves, dfs_f(row, col - 1, t + 1));
    }
    // Is it safe to stay put?
    if (row < 0 || is_safe(row, col, t + 1)) {
        min_moves = min(min_moves, dfs_f(row, col, t + 1));
    }
    cache[t * rows * cols + row * cols + col] = min_moves;
    g_min_moves = min(g_min_moves, min_moves);
    return min_moves;
}

// Depth-first search going backward
static ll dfs_b(ll row, ll col, ll t)
{
    auto iter = cache.find(t * rows * cols + row * cols + col);
    if (iter != cache.end()) return iter->second;
    if (row == 0 && col == 0) {
        return t + 1;
    }
    ll min_moves = INT64_MAX;
    // If we already found a better minimum return directly
    if (t + 1 >= g_min_moves) {
        return min_moves;
    }
    // Search left and up moves first since they lead to the goal
    if (row - 1 >= 0 && is_safe(row - 1, col, t + 1)) {
        min_moves = min(min_moves, dfs_b(row - 1, col, t + 1));
    }
    if (col - 1 >= 0 && row < rows && is_safe(row, col - 1, t + 1)) {
        min_moves = min(min_moves, dfs_b(row, col - 1, t + 1));
    }
    if (row + 1 < rows && is_safe(row + 1, col, t + 1)) {
        min_moves = min(min_moves, dfs_b(row + 1, col, t + 1));
    }
    if (col + 1 < cols && row < rows && is_safe(row, col + 1, t + 1)) {
        min_moves = min(min_moves, dfs_b(row, col + 1, t + 1));
    }
    // Is it safe to stay put?
    if (row >= rows || is_safe(row, col, t + 1)) {
        min_moves = min(min_moves, dfs_b(row, col, t + 1));
    }
    cache[t * rows * cols + row * cols + col] = min_moves;
    g_min_moves = min(g_min_moves, min_moves);
    return min_moves;
}

int main()
{
    // Parse the input
    vector<string> start_grid;
    string line;
    assert(getline(cin, line));
    while (getline(cin, line)) {
        assert(line.size() > 1);
        if (line[1] == '#') break;
        line = line.substr(1, line.size() - 2);
        if (!start_grid.empty()) assert(line.size() == start_grid[0].size());
        start_grid.push_back(line);
    }
    assert(!getline(cin, line));
    rows = ll(start_grid.size());
    cols = ll(start_grid[0].size());

    // Fill the blizzard vectors
    for (ll row = 0; row < rows; ++row) {
        h_bliz.emplace_back(0);
        for (ll col = 0; col < cols; ++col) {
            if (row == 0) {
                v_bliz.emplace_back(0);
            }
            char c = start_grid[row][col];
            // Negative values represent blizzards moving in the negative direction
            // We add one or subtract one to avoid zero
            switch (c) {
            case '<':
                h_bliz[row].push_back(-col - 1);
                break;
            case '>':
                h_bliz[row].push_back(col + 1);
                break;
            case '^':
                v_bliz[col].push_back(-row - 1);
                break;
            case 'v':
                v_bliz[col].push_back(row + 1);
                break;
            default:
                assert(c == '.');
            }
        }
    }

    // Go forward, backward and forward again
    ll p1 = dfs_f(-1, 0, 0);
    cache.clear();
    g_min_moves = INT64_MAX;
    ll p2 = dfs_b(rows, cols - 1, p1);
    cache.clear();
    g_min_moves = INT64_MAX;
    p2 = dfs_f(-1, 0, p2);

    cout << "Part 1: " << p1 << endl;
    cout << "Part 2: " << p2 << endl;

    return 0;
}
