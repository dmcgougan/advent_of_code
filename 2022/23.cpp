/*
 * Problem 23, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using ll = int64_t;

// Set of elves
static set<pair<ll, ll>> elves;

// Map of movement proposals
static map<pair<ll, ll>, pair<ll, ll>> propose;

// Convert a direction to delta-row and delta-column
static pair<ll, ll> delta(ll dir)
{
    static constexpr pair<ll, ll> d[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    assert(dir >= 0 && dir < 4);
    return d[dir];
}

static bool round(ll start_dir)
{
    static constexpr pair<ll, ll> d8[] =
        {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

    // First half of round
    for (auto [row, col] : elves) {
        // Check if we find any elves around this one
        bool empty = true;
        for (auto [drow, dcol] : d8) {
            if (elves.count(make_pair(row + drow, col + dcol))) {
                empty = false;
                break;
            }
        }
        // If not, then this elf stays still
        if (empty) continue;

        // Check if we should propose to move somewhere
        ll drow, dcol;
        ll dir = start_dir;
        for (ll i = 0; i < 4; ++i, dir = (dir + 1) % 4) {
            tie(drow, dcol) = delta(dir);
            if (elves.count(make_pair(row + drow, col + dcol))) {
                continue;
            }
            if (elves.count(make_pair(row + (drow ? drow : -1), col + (dcol ? dcol : -1)))) {
                continue;
            }
            if (elves.count(make_pair(row + (drow ? drow : 1), col + (dcol ? dcol : 1)))) {
                continue;
            }
            // No elves in this general direction; propose to go here
            auto iter = propose.find(make_pair(row + drow, col + dcol));
            if (iter == propose.end()) {
                // No other elf has yet proposed to go here
                propose[make_pair(row + drow, col + dcol)] = make_pair(row, col);
            } else {
                // Another elf has proposed this position; mark it as unavailable
                iter->second = make_pair(INT64_MAX, 0);
            }
            break;
        }
    }

    // Second half of round
    bool moved = false;
    for (const auto& [dst, src] : propose) {
        if (src.first != INT64_MAX) {
            // We found a movement proposal that was exclusive to one elf; move it
            elves.erase(make_pair(src.first, src.second));
            elves.insert(make_pair(dst.first, dst.second));
            moved = true;
        }
    }
    propose.clear();

    // Return true iff at least one elf moved
    return moved;
}

// Get the dimensions of the current grid
static void dimensions(ll& min_row, ll& max_row, ll& min_col, ll& max_col)
{
    min_row = INT64_MAX;
    max_row = INT64_MIN;
    min_col = INT64_MAX;
    max_col = INT64_MIN;
    for (auto [row, col] : elves) {
        min_row = min(min_row, row);
        max_row = max(max_row, row);
        min_col = min(min_col, col);
        max_col = max(max_col, col);
    }
}

// Count how much empty space there is in the current grid
static ll count_space()
{
    ll count = 0;
    ll min_row, max_row, min_col, max_col;
    dimensions(min_row, max_row, min_col, max_col);
    for (ll row = min_row; row <= max_row; ++row) {
        for (ll col = min_col; col <= max_col; ++col) {
            if (!elves.count(make_pair(row, col))) count++;
        }
    }
    return count;
}

int main()
{
    // Parse the input
    string line;
    ll row = 0;
    while (getline(cin, line)) {
        for (ll col = 0; col < ll(line.size()); ++col) {
            if (line[col] == '#') {
                elves.insert(make_pair(row, col));
            }
        }
        row++;
    }

    // Run until no elves move
    ll dir = 0;
    ll round_ix = 1;
    while (round(dir)) {
        if (round_ix == 10) {
            cout << "Part 1: " << count_space() << endl;
        }
        round_ix++;
        dir = (dir + 1) % 4;
    }

    cout << "Part 2: " << round_ix << endl;

    return 0;
}
