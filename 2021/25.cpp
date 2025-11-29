/*
 * Problem 25, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

static bool step(vector<string>& grid)
{
    vector<tuple<int, int, char>> update;
    int rows = grid.size();
    int cols = grid[0].size();
    bool moved = false;

    // Move east
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '>') {
                int nc = (c + 1) % cols;
                if (grid[r][nc] == '.') {
                    update.push_back({r, c, '.'});
                    update.push_back({r, nc, '>'});
                    moved = true;
                }
            }
        }
    }
    for (const auto& [r, c, ch] : update) {
        grid[r][c] = ch;
    }
    update.clear();

    // Move south
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'v') {
                int nr = (r + 1) % rows;
                if (grid[nr][c] == '.') {
                    update.push_back({r, c, '.'});
                    update.push_back({nr, c, 'v'});
                    moved = true;
                }
            }
        }
    }
    for (const auto& [r, c, ch] : update) {
        grid[r][c] = ch;
    }

    return moved;
}

int main()
{
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.push_back(move(line));
    }
    assert(!grid.empty());

    int part1 = 1;
    while (step(grid)) ++part1;

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
