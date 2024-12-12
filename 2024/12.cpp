/*
 * Problem 12, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

static vector<vector<bool>> visited;
static vector<vector<int>> fence;
static vector<string> grid;
static int rows, cols;
static int area, perimeter;

// DFS walk of the grid to find an area of the same plant
static void count(int r, int c)
{
    if (visited[r][c]) return;
    visited[r][c] = true;
    char plant = grid[r][c];
    area++;

    for (int d = 0; d < 4; d++) {
        int nr = r + dirs[d][0];
        int nc = c + dirs[d][1];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == plant) {
            count(nr, nc);
        } else {
            // Mark this side of the cell as having a fence
            fence[r][c] |= (1 << d);
            // Count perimeter for part 1
            perimeter++;
        }
    }
}

// Count consecutive stretches of fence
static int fence_sides()
{
    int sides = 0;

    // Count horizontal stretches of fence
    for (int r = 0; r < rows; ++r) {
        // Check both upper and lower sides of the row
        for (int d = 0; d < 2; ++d) {
            bool counted = false;
            for (int c = 0; c < cols; ++c) {
                if (fence[r][c] & (1 << (2 * d + 1))) {
                    if (!counted) {
                        // Count and mark this stretch as counted
                        sides++;
                        counted = true;
                    }
                } else {
                    // Count the next segment if the stretch is interrupted
                    counted = false;
                }
            }
        }
    }

    // Count vertical stretches of fence
    for (int c = 0; c < cols; ++c) {
        // Check both left and right sides of the column
        for (int d = 0; d < 2; ++d) {
            bool counted = false;
            for (int r = 0; r < rows; ++r) {
                if (fence[r][c] & (1 << (2 * d))) {
                    if (!counted) {
                        // Count and mark this stretch as counted
                        sides++;
                        counted = true;
                    }
                } else {
                    // Count the next segment if the stretch is interrupted
                    counted = false;
                }
            }
        }
    }
    return sides;
}

int main()
{
    string line;
    while (getline(cin, line)) {
        grid.emplace_back(move(line));
    }
    rows = grid.size();
    cols = grid[0].size();
    visited = vector<vector<bool>>(rows, vector<bool>(cols));

    int part1 = 0;
    int part2 = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c]) {
                area = 0;
                perimeter = 0;
                fence = vector<vector<int>>(rows, vector<int>(cols));
                count(r, c);
                part1 += area * perimeter;
                part2 += area * fence_sides();
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
