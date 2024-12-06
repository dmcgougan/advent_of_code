/*
 * Problem 6, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

// Count number of visited positions; 0 for infinity (loop)
static int visit_count(const vector<string>& grid, int r, int c)
{
    static const int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<int>> visited(rows, vector<int>(cols));
    int count = 0;
    int d = 0;
    for (;;) {
        // Count the current position if we've never been here
        if (visited[r][c] == 0) count++;
        // Mark the direction we are facing at this position
        visited[r][c] |= (1 << d);
        for (;;) {
            // Potential new position
            int nr = r + dirs[d][0];
            int nc = c + dirs[d][1];
            // Check if we are moving outside the map
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) return count;
            if (grid[nr][nc] != '#') {
                // Move
                r = nr;
                c = nc;
                break;
            }
            // Try next direction
            d = (d + 1) % 4;
        }
        // If we came to this position facing the same direction before, then we have a loop
        if (visited[r][c] & (1 << d)) return 0;
    }
}

int main()
{
    vector<string> grid;
    int start_row = -1;
    int start_col = -1;
    string line;
    while (getline(cin, line)) {
        size_t i = line.find_first_of('^');
        if (i != string::npos) {
            start_row = grid.size();
            start_col = i;
        }
        grid.emplace_back(line);
    }
    assert(start_row >= 0);
    int rows = grid.size();
    int cols = grid[0].size();

    // Part 1
    int part1 = visit_count(grid, start_row, start_col);

    // Part 2
    int part2 = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] != '.') continue;
            // Add obstruction
            grid[r][c] = '#';
            // Check for loop
            if (visit_count(grid, start_row, start_col) == 0) part2++;
            grid[r][c] = '.';
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
