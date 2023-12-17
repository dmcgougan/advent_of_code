/*
 * Problem 17, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

static int solve(const vector<string>& grid, int min_moves, int max_moves)
{
    int rows = grid.size();
    int cols = grid[0].size();
    priority_queue<array<int, 6>, std::vector<array<int, 6>>, std::greater<array<int, 6>>> q;
    set<array<int, 5>> seen;

    // We can start in two directions
    q.push({0, 0, 0, 0, 0, 1});
    q.push({0, 0, 0, 0, 1, 0});

    // Process the possible moves in order of least loss so far
    // That guarantees that we found the best path when we see the goal tile
    while (!q.empty()) {
        auto [loss, r, c, m, dr, dc] = q.top();
        q.pop();
        if (r == rows - 1 && c == cols - 1 && m >= min_moves) {
            return loss;
        }
        if (seen.count({r, c, m, dr, dc})) continue;
        seen.insert({r, c, m, dr, dc});
        if (m < max_moves && r + dr >= 0 && r + dr < rows && c + dc >= 0 && c + dc < cols) {
            // Move in the current direction
            q.push({loss + grid[r + dr][c + dc] - '0', r + dr, c + dc, m + 1, dr, dc});
        }
        if (m >= min_moves) {
            // Turn either left or right
            q.push({loss, r, c, 0, dc, dr});
            q.push({loss, r, c, 0, -dc, -dr});
        }
    }

    assert(false);
}

int main()
{
    // Parse input data
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        grid.push_back(line);
    }

    // Part 1
    int part1 = solve(grid, 1, 3);

    // Part 2
    int part2 = solve(grid, 4, 10);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
