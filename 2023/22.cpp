/*
 * Problem 22, Advent of Code 2023
 * Danjel McGougan
 */

#include <stdio.h>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static int max_x;
static int max_y;
static int max_z;
static int bricks;

static vector<vector<vector<int>>> grid;
static vector<array<int, 6>> input;

static bool fall(int brick, bool commit = true)
{
    bool falling = false;
    auto [x0, y0, z0, x1, y1, z1] = input[brick];
    if (x0 != x1) {
        if (z0 > 1) {
            bool in_air = true;
            for (int x = x0; x <= x1; ++x) {
                assert(grid[x][y0][z0] == brick);
                if (grid[x][y0][z0 - 1] >= 0) {
                    in_air = false;
                    break;
                }
            }
            if (in_air) {
                falling = true;
                if (commit) {
                    for (int x = x0; x <= x1; ++x) {
                        grid[x][y0][z0] = -1;
                        grid[x][y0][z0 - 1] = brick;
                    }
                }
            }
        }
    } else if (y0 != y1) {
        if (z0 > 1) {
            bool in_air = true;
            for (int y = y0; y <= y1; ++y) {
                assert(grid[x0][y][z0] == brick);
                if (grid[x0][y][z0 - 1] >= 0) {
                    in_air = false;
                    break;
                }
            }
            if (in_air) {
                falling = true;
                if (commit) {
                    for (int y = y0; y <= y1; ++y) {
                        grid[x0][y][z0] = -1;
                        grid[x0][y][z0 - 1] = brick;
                    }
                }
            }
        }
    } else if (z0 != z1) {
        if (min(z0, z1) > 1) {
            if (grid[x0][y0][min(z0, z1) - 1] < 0) {
                falling = true;
                if (commit) {
                    assert(grid[x0][y0][z0] == brick);
                    assert(grid[x0][y0][z1] == brick);
                    grid[x0][y0][max(z0, z1)] = -1;
                    grid[x0][y0][min(z0, z1) - 1] = brick;
                }
            }
        }
    } else if (z0 > 1) {
        if (grid[x0][y0][z0 - 1] < 0) {
            falling = true;
            if (commit) {
                assert(grid[x0][y0][z0] == brick);
                grid[x0][y0][z0] = -1;
                grid[x0][y0][z0 - 1] = brick;
            }
        }
    }
    if (falling && commit) {
        input[brick] = {x0, y0, z0 - 1, x1, y1, z1 - 1};
    }
    return falling;
}

int main()
{
    // Parse input data
    string line;
    while (getline(cin, line)) {
        int x0, y0, z0, x1, y1, z1;
        sscanf(line.c_str(), "%d,%d,%d~%d,%d,%d", &x0, &y0, &z0, &x1, &y1, &z1);
        input.push_back({x0, y0, z0, x1, y1, z1});
        max_x = max(max_x, x0);
        max_x = max(max_x, x1);
        max_y = max(max_y, y0);
        max_y = max(max_y, y1);
        max_z = max(max_z, z0);
        max_z = max(max_z, z1);
    }
    bricks = input.size();
    grid = vector(max_x + 1, vector(max_y + 1, vector(max_z + 1, -1)));
    for (size_t i = 0; i < input.size(); ++i) {
        auto [x0, y0, z0, x1, y1, z1] = input[i];
        if (x0 != x1) {
            for (int x = x0; x <= x1; ++x) {
                assert(grid[x][y0][z0] < 0);
                grid[x][y0][z0] = i;
            }
        } else if (y0 != y1) {
            for (int y = y0; y <= y1; ++y) {
                assert(grid[x0][y][z0] < 0);
                grid[x0][y][z0] = i;
            }
        } else if (z0 != z1) {
            for (int z = z0; z <= z1; ++z) {
                assert(grid[x0][y0][z] < 0);
                grid[x0][y0][z] = i;
            }
        } else {
            grid[x0][y0][z0] = i;
        }
    }

    // Let all bricks fall
    bool something_falling;
    do {
        something_falling = false;
        for (int b = 0; b < bricks; ++b) {
            if (fall(b)) {
                something_falling = true;
            }
        }
    } while (something_falling);

    // Check bricks for those that can be removed without anything falling
    vector<vector<vector<int>>> grid_copy = grid;
    vector<array<int, 6>> input_copy = input;
    int part1 = 0;
    int part2 = 0;
    for (int b = 0; b < bricks; ++b) {
        auto [x0, y0, z0, x1, y1, z1] = input[b];
        // Disintegrate brick b and see what happens
        if (x0 != x1) {
            for (int x = x0; x <= x1; ++x) {
                assert(grid[x][y0][z0] == b);
                grid[x][y0][z0] = -1;
            }
        } else if (y0 != y1) {
            for (int y = y0; y <= y1; ++y) {
                assert(grid[x0][y][z0] == b);
                grid[x0][y][z0] = -1;
            }
        } else if (z0 != z1) {
            for (int z = z0; z <= z1; ++z) {
                assert(grid[x0][y0][z] == b);
                grid[x0][y0][z] = -1;
            }
        } else {
            assert(grid[x0][y0][z0] == b);
            grid[x0][y0][z0] = -1;
        }
        bool is_safe = true;
        for (int b0 = 0; b0 < bricks; ++b0) {
            if (b0 != b && fall(b0, false)) {
                is_safe = false;
                break;
            }
        }
        if (is_safe) {
            // Nothing moved
            ++part1;
        } else {
            // Something fell
            // Let everything fall to rest and see how many bricks are affected
            bool something_falling;
            vector<bool> fell(bricks, false);
            do {
                something_falling = false;
                for (int b0 = 0; b0 < bricks; ++b0) {
                    if (b0 != b && fall(b0)) {
                        something_falling = true;
                        fell[b0] = true;
                    }
                }
            } while (something_falling);
            for (int b0 = 0; b0 < bricks; ++b0) {
                if (fell[b0]) ++part2;
            }
        }

        // Restore the grid
        grid = grid_copy;
        input = input_copy;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
