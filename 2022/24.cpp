/*
 * Problem 24, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <unordered_set>
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
    return !bliz_left[row][(col + t) % cols] && !bliz_right[row][mod_neg(col - t, cols)] &&
        !bliz_up[(row + t) % rows][col] && !bliz_down[mod_neg(row - t, rows)][col];
}

static ll search_f(ll start_t)
{
    ll min_moves = INT64_MAX;
    deque<tuple<ll, ll, ll>> q;
    unordered_set<ll> seen;
    q.push_back(make_tuple(-1, 0, start_t));
    while (!q.empty()) {
        auto [row, col, t] = q.front();
        q.pop_front();
        if (row == rows - 1 && col == cols - 1) {
            min_moves = min(min_moves, t + 1);
            continue;
        }
        if (t + (cols - col - 1) + (rows - row - 1) + 1 >= min_moves) {
            continue;
        }
        if (seen.count(t * rows * cols + row * cols + col)) {
            continue;
        }
        seen.insert(t * rows * cols + row * cols + col);
        // Search right and down first since those lead to the goal
        if (row + 1 < rows && is_safe(row + 1, col, t + 1)) {
            q.push_front(make_tuple(row + 1, col, t + 1));
        }
        if (col + 1 < cols && row >= 0 && is_safe(row, col + 1, t + 1)) {
            q.push_front(make_tuple(row, col + 1, t + 1));
        }
        if (row < 0 || is_safe(row, col, t + 1)) {
            q.push_back(make_tuple(row, col, t + 1));
        }
        if (row - 1 >= 0 && is_safe(row - 1, col, t + 1)) {
            q.push_back(make_tuple(row - 1, col, t + 1));
        }
        if (col - 1 >= 0 && row >= 0 && is_safe(row, col - 1, t + 1)) {
            q.push_back(make_tuple(row, col - 1, t + 1));
        }
    }
    return min_moves;
}

static ll search_b(ll start_t)
{
    ll min_moves = INT64_MAX;
    deque<tuple<ll, ll, ll>> q;
    unordered_set<ll> seen;
    q.push_back(make_tuple(rows, cols - 1, start_t));
    while (!q.empty()) {
        auto [row, col, t] = q.front();
        q.pop_front();
        if (row == 0 && col == 0) {
            min_moves = min(min_moves, t + 1);
            continue;
        }
        if (t + col + row + 1 >= min_moves) {
            continue;
        }
        if (seen.count(t * rows * cols + row * cols + col)) {
            continue;
        }
        seen.insert(t * rows * cols + row * cols + col);
        // Search left and up first since those lead to the goal
        if (row - 1 >= 0 && is_safe(row - 1, col, t + 1)) {
            q.push_front(make_tuple(row - 1, col, t + 1));
        }
        if (col - 1 >= 0 && row < rows && is_safe(row, col - 1, t + 1)) {
            q.push_front(make_tuple(row, col - 1, t + 1));
        }
        if (row == rows || is_safe(row, col, t + 1)) {
            q.push_back(make_tuple(row, col, t + 1));
        }
        if (row + 1 < rows && is_safe(row + 1, col, t + 1)) {
            q.push_back(make_tuple(row + 1, col, t + 1));
        }
        if (col + 1 < cols && row < rows && is_safe(row, col + 1, t + 1)) {
            q.push_back(make_tuple(row, col + 1, t + 1));
        }
    }
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
    ll p1 = search_f(0);
    ll p2 = search_b(p1);
    p2 = search_f(p2);

    cout << "Part 1: " << p1 << endl;
    cout << "Part 2: " << p2 << endl;

    return 0;
}
