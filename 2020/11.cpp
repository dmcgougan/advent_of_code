/*
 * Problem 11, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static bool step_p1(vector<string>& grid)
{
    vector<pair<int, int>> toggles;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char seat = grid[r][c];
            if (seat == '.') continue;
            int occ = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (!dr && !dc) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    if (grid[nr][nc] == '#') ++occ;
                }
            }
            if (seat == 'L' && occ == 0) toggles.push_back({r, c});
            if (seat == '#' && occ >= 4) toggles.push_back({r, c});
        }
    }
    for (auto [r, c] : toggles) {
        grid[r][c] = (grid[r][c] == 'L' ? '#' : 'L');
    }
    return !toggles.empty();
}

static bool step_p2(vector<string>& grid)
{
    vector<pair<int, int>> toggles;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char seat = grid[r][c];
            if (seat == '.') continue;
            int occ = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (!dr && !dc) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    while (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        if (grid[nr][nc] == '#') {
                            ++occ;
                            break;
                        }
                        if (grid[nr][nc] == 'L') break;
                        nr += dr;
                        nc += dc;
                    }
                }
            }
            if (seat == 'L' && occ == 0) toggles.push_back({r, c});
            if (seat == '#' && occ >= 5) toggles.push_back({r, c});
        }
    }
    for (auto [r, c] : toggles) {
        grid[r][c] = (grid[r][c] == 'L' ? '#' : 'L');
    }
    return !toggles.empty();
}

static int count_occ(const vector<string>& grid)
{
    int count = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            count += (grid[r][c] == '#');
        }
    }
    return count;
}

int main()
{
    // Parse input
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.push_back(move(line));
    }
    assert(!grid.empty());

    // Solve part 1
    vector g(grid);
    while (step_p1(g))
        ;
    int part1 = count_occ(g);

    // Solve part 2
    while (step_p2(grid))
        ;
    int part2 = count_occ(grid);

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
