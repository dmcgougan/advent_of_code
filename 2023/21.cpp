/*
 * Problem 21, Advent of Code 2023
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static inline int mod(int i, int n) { return (i % n + n) % n; }

static vector<string> tile;
static map<array<int, 2>, int> depth_map;
static int rows;
static int cols;
static int start_row;
static int start_col;

// Calculate how many cells that can be reached in the specified number of steps
static ll solve(int steps)
{
    ll count = 0;

    // For each cell in the depth map calculate how many world cells
    // corresponding to that cell that can be reached
    for (auto [pos, d] : depth_map) {
        auto [r, c] = pos;

        if (d > steps) continue;

        // Count the cell itself if reachable
        if ((d % 2) == (steps % 2)) count++;

        // Check if this cell is on an edge tile of the depth map
        bool row_edge = (r < -2 * rows || r >= 3 * rows);
        bool col_edge = (c < -2 * cols || c >= 3 * cols);
        if (row_edge || col_edge) {
            // Check if we are on a reachable cell
            int odd = !(d % 2 == steps % 2);

            // Calculate how many tiles in the cardinal direction we should add.
            // This works because the perimeter is empty so the tiles we
            // are adding here can be reached by just adding walking distance
            // without obstructions. The final walk into the interior of the
            // tile has already been accounted for by the walking we did within
            // the depth map. In other words for the tiles outside the depth
            // map we do not go into the interior directly, we just continue
            // walking along an unobstructed path until we get near the destination
            // tile, then we walk into the interior (which we already accounted
            // for). The puzzle input also has free pathways along the center in
            // each cardinal direction from the starting point, so this reduces
            // the size of the needed depth map.
            ll n = ((steps - d) / rows + odd) / 2;
            if (!row_edge || !col_edge) {
                // Edge tile - add only for the cardinal direction
                count += n;
            } else {
                // Corner tile - add for two cardinal directions
                count += n * (n + 2 - odd);
            }
        }
    }

    return count;
}

int main()
{
    // Parse input data
    string line;
    int r = 0;
    start_row = -1;
    while (getline(cin, line)) {
        size_t c;
        if (start_row < 0 && (c = line.find('S')) != string::npos) {
            start_row = r;
            start_col = c;
        }
        ++r;
        tile.push_back(move(line));
    }
    rows = tile.size();
    cols = tile[0].size();
    assert(start_row >= 0);
    assert(tile[start_row][start_col] == 'S');
    assert(cols == rows);  // square tiles
    assert(rows % 2);      // odd width and height of a tile

    // Terminology:
    // tile - the puzzle input
    // world - the tiles laid out infinitely in all directions
    // cell - one cell in the world

    // Calculate a depth map containing the number of steps needed to reach each
    // cell going 3 tiles out in all directions from the starting tile.
    // (so the resulting depth map covers 7 by 7 tiles)
    deque<array<int, 3>> q;
    q.push_back({start_row, start_col, 0});
    while (!q.empty()) {
        auto [r, c, d] = q.front();
        q.pop_front();

        // Check if we've gone outside the 7 by 7 tiles
        if (r < -3 * rows || r >= 4 * rows) continue;
        if (c < -3 * cols || c >= 4 * cols) continue;

        if (depth_map.find({r, c}) != depth_map.end()) continue;

        depth_map[{r, c}] = d;

        if (tile[mod(r - 1, rows)][mod(c, cols)] != '#') {
            q.push_back({r - 1, c, d + 1});
        }
        if (tile[mod(r + 1, rows)][mod(c, cols)] != '#') {
            q.push_back({r + 1, c, d + 1});
        }
        if (tile[mod(r, rows)][mod(c - 1, cols)] != '#') {
            q.push_back({r, c - 1, d + 1});
        }
        if (tile[mod(r, rows)][mod(c + 1, cols)] != '#') {
            q.push_back({r, c + 1, d + 1});
        }
    }

    ll part1 = solve(64);
    ll part2 = solve(26501365);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
