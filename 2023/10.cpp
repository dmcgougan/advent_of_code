/*
 * Problem 10, Advent of Code 2023
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

static vector<string> mymap;
static vector<string> bigmap;
static int rows;
static int cols;

enum direction_t
{
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
    DIR_NORTH
};

static const vector<array<int, 2>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

static const map<char, vector<direction_t>> legend = {
    {'|', {DIR_NORTH, DIR_SOUTH}},
    {'-', {DIR_EAST, DIR_WEST}},
    {'L', {DIR_NORTH, DIR_EAST}},
    {'J', {DIR_NORTH, DIR_WEST}},
    {'7', {DIR_SOUTH, DIR_WEST}},
    {'F', {DIR_SOUTH, DIR_EAST}},
};

// Traverse the loop and mark loop tiles with '*' on the double size map
static int loop_length(int row, int col)
{
    // Find starting direction
    int sdir = -1;
    for (int dir = 0; dir < 4; ++dir) {
        auto [drow, dcol] = directions[dir];
        int r = row + drow;
        int c = col + dcol;
        if (r < 0 || r >= rows || c < 0 || c >= cols) continue;
        auto iter = legend.find(mymap[r][c]);
        if (iter == legend.end()) continue;
        int odir = (dir + 2) % 4;
        if (iter->second[0] == odir || iter->second[1] == odir) {
            sdir = dir;
            break;
        }
    }

    // Traverse the loop and count its length
    int r = row;
    int c = col;
    int d = sdir;
    int steps = 0;
    for (;;) {
        // Mark current location and the location in the direction of the next location
        assert(r >= 0 && r < rows && c >= 0 && c < cols);
        bigmap[r * 2 + 1][c * 2 + 1] = '*';
        bigmap[r * 2 + 1 + directions[d][0]][c * 2 + 1 + directions[d][1]] = '*';

        // Move
        int pr = r;  // previous row
        int pc = c;  // previous col
        r += directions[d][0];
        c += directions[d][1];
        ++steps;

        // Check if we are back at the start
        if (r == row && c == col) break;

        // Find new direction
        auto iter = legend.find(mymap[r][c]);
        assert(iter != legend.end());
        d = iter->second[0];
        int nr = r + directions[d][0];
        int nc = c + directions[d][1];
        if (nr == pr && nc == pc) {
            d = iter->second[1];
        }
    }

    return steps;
}

static void mark_outside_tiles(int rows, int cols)
{
    // Traverse all reachable tiles of the large map starting
    // from the upper left corner. The loop tiles are barriers.
    assert(bigmap[0][0] == '.');
    deque<array<int, 2>> q;
    bigmap[0][0] = 'O';
    q.push_back({0, 0});
    while (!q.empty()) {
        auto [row, col] = q.front();
        q.pop_front();
        for (int d = 0; d < 4; ++d) {
            int nr = row + directions[d][0];
            int nc = col + directions[d][1];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            char c = bigmap[nr][nc];
            if (c != '*' && c != 'O') {
                // Found a new tile to go to - mark it as an outside tile
                q.push_back({nr, nc});
                bigmap[nr][nc] = 'O';
            }
        }
    }
}

int main()
{
    // Parse input data and find S
    string line;
    int srow = -1;
    int scol = -1;
    while (getline(cin, line)) {
        size_t spos = line.find('S');
        if (spos != string::npos) {
            scol = spos;
            srow = mymap.size();
        }
        mymap.emplace_back(move(line));
    }
    rows = mymap.size();
    cols = mymap[0].size();
    assert(srow >= 0 && scol >= 0);

    // Make a larger map that has room between all pipes and along the perimeter
    bigmap = vector<string>(rows * 2 + 1, string(cols * 2 + 1, '.'));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            bigmap[row * 2 + 1][col * 2 + 1] = mymap[row][col];
        }
    }

    // Part 1 - Traverse the loop and mark all loop tiles on the larger map
    int part1 = loop_length(srow, scol) / 2;

    // Part 2 - Mark the outside tiles and then count the inside tiles
    mark_outside_tiles(rows * 2 + 1, cols * 2 + 1);
    int part2 = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            char c = bigmap[row * 2 + 1][col * 2 + 1];
            if (c != 'O' && c != '*') ++part2;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
