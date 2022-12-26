/*
 * Problem 24, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Blizzards
static ll rows, cols;
static vector<vector<bool>> bliz_left;
static vector<vector<bool>> bliz_right;
static vector<vector<bool>> bliz_up;
static vector<vector<bool>> bliz_down;

static inline ll mod_neg(ll a, ll b)
{
    ll m = a % b;
    return m >= 0 ? m : m + b;
}

// Is the given row and column safe at time t?
static inline bool is_safe(ll row, ll col, ll t)
{
    return
        !bliz_left[row][(col + t) % cols] &&
        !bliz_right[row][mod_neg(col - t, cols)] &&
        !bliz_up[(row + t) % rows][col] &&
        !bliz_down[mod_neg(row - t, rows)][col];
}

// DP memoize
static unordered_map<ll, ll> cache;
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
    if (t + (cols - col - 1) + (rows - row - 1) + 1 >= g_min_moves) {
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
    if (t + col + row + 1 >= g_min_moves) {
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
    bliz_left = vector<vector<bool>>(rows, vector<bool>(cols, false));
    bliz_right = vector<vector<bool>>(rows, vector<bool>(cols, false));
    bliz_up = vector<vector<bool>>(rows, vector<bool>(cols, false));
    bliz_down = vector<vector<bool>>(rows, vector<bool>(cols, false));
    for (ll row = 0; row < rows; ++row) {
        for (ll col = 0; col < cols; ++col) {
            char c = start_grid[row][col];
            switch (c) {
            case '<':
                bliz_left[row][col] = true;
                break;
            case '>':
                bliz_right[row][col] = true;
                break;
            case '^':
                bliz_up[row][col] = true;
                break;
            case 'v':
                bliz_down[row][col] = true;
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
