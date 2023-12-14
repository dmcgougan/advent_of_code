/*
 * Problem 14, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Turn rows into columns and columns into rows
static vector<string> zip(const vector<string>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();
    vector<string> ret(vector<string>(cols, string(rows, ' ')));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            ret[c][r] = grid[r][c];
        }
    }
    return ret;
}

// Calculate load value for the grid
static int load(const vector<string>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();
    int ret = 0;
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] == 'O') {
                ret += rows - r;
            }
        }
    }
    return ret;
}

// Tilt the grid in direction dir
static void tilt(int dir, vector<string>& grid)
{
    if (dir % 2) grid = zip(grid);

    int rows = grid.size();
    int cols = grid[0].size();

    for (int c = 0; c < cols; ++c) {
        if (dir / 2) {
            bool moved;
            do {
                moved = false;
                for (int r0 = 0; r0 < rows; ++r0) {
                    if (grid[r0][c] == 'O') {
                        int r;
                        for (r = r0; r + 1 < rows; ++r) {
                            if (grid[r + 1][c] != '.') break;
                        }
                        if (r != r0) {
                            grid[r0][c] = '.';
                            grid[r][c] = 'O';
                            moved = true;
                        }
                    }
                }
            } while (moved);
        } else {
            bool moved;
            do {
                moved = false;
                for (int r0 = rows - 1; r0 >= 0; --r0) {
                    if (grid[r0][c] == 'O') {
                        int r;
                        for (r = r0; r > 0; r--) {
                            if (grid[r - 1][c] != '.') break;
                        }
                        if (r != r0) {
                            grid[r0][c] = '.';
                            grid[r][c] = 'O';
                            moved = true;
                        }
                    }
                }
            } while (moved);
        }
    }

    if (dir % 2) grid = zip(grid);
}

// Calculate a hash value of the grid
static uint64_t gridhash(const vector<string>& grid)
{
    uint64_t h = 5381;
    for (const string& s : grid) {
        for (char c : s) {
            h = 33 * h + c;
        }
    }
    return h;
}

int main()
{
    vector<string> grid;

    // Parse input data
    string line;
    while (getline(cin, line)) {
        grid.push_back(line);
    }

    // Part 1
    tilt(0, grid);
    int part1 = load(grid);

    // Map from grid hash to round
    map<ll, int> states;

    // Part 2
    bool almost_done = false;
    for (int round = 0; round < 1000000000; ++round) {
        // Do one tilt cycle
        for (int dir = 0; dir < 4; ++dir) tilt(dir, grid);
        if (almost_done) continue;

        // Calculate a hash of the grid state
        uint64_t h = gridhash(grid);

        // Have we seen this state before?
        auto iter = states.find(h);
        if (iter == states.end()) {
            // No, remember this state
            states.insert(make_pair(h, round));
        } else {
            // Yes! We have seen this state before.
            // Check how many rounds ago that was.
            // So if we go the same number of rounds into the future
            // we will end up at the same state again.
            int prev_round = iter->second;
            int diff = round - prev_round;
            // So now add rounds until we are almost done
            round += (1000000000 - round) / diff * diff;
            almost_done = true;
        }
    }
    int part2 = load(grid);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
