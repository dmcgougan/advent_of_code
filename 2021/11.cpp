/*
 * Problem 11, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static const pair<int, int> dirs[] =
    {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

static int step(vector<string>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            ++grid[r][c];
        }
    }
    int count = 0;
    vector<vector<bool>> flashed(rows, vector<bool>(cols, false));
    bool any_flash;
    do {
        any_flash = false;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (!flashed[r][c] && grid[r][c] > '9') {
                    grid[r][c] = '0';
                    flashed[r][c] = true;
                    ++count;
                    any_flash = true;
                    for (auto [dr, dc] : dirs) {
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                        if (flashed[nr][nc]) continue;
                        ++grid[nr][nc];
                    }
                }
            }
        }
    } while (any_flash);
    return count;
}

int main()
{
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.push_back(move(line));
    }
    int tiles = grid.size() * grid[0].size();

    int part1 = 0;
    int part2 = 0;
    int s;
    for (s = 0; s < 100; ++s) {
        int count = step(grid);
        part1 += count;
        if (!part2 && count == tiles) part2 = s + 1;
    }
    for (; !part2; ++s) {
        if (step(grid) == tiles) part2 = s + 1;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
