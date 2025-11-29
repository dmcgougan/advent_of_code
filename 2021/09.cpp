/*
 * Problem 9, Advent of Code 2021
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

static const vector<pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

static int basin_size(const vector<string>& grid, int row, int col)
{
    int count = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<int, int>> q;
    q.push({row, col});
    visited[row][col] = true;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        count++;
        for (auto [dr, dc] : dirs) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (visited[nr][nc] || grid[nr][nc] == '9') continue;
            q.push({nr, nc});
            visited[nr][nc] = true;
        }
    }
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
    int rows = grid.size();
    int cols = grid[0].size();

    vector<int> sizes;
    int part1 = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            char ch = grid[r][c];
            bool low = true;
            for (auto [dr, dc] : dirs) {
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && ch >= grid[nr][nc]) {
                    low = false;
                    break;
                }
            }
            if (low) {
                // Found a low point
                part1 += ch - '0' + 1;
                // Calculate the size of the basin
                sizes.push_back(basin_size(grid, r, c));
            }
        }
    }

    // Get the three largest basin sizes and multiply them together
    int part2 = 1;
    sort(sizes.begin(), sizes.end(), greater<int>());
    for (size_t i = 0; i < 3 && i < sizes.size(); i++) {
        part2 *= sizes[i];
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
