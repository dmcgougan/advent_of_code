/*
 * Problem 16, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<string> grid;
static vector<vector<int>> visited;
static int rows;
static int cols;

static int run_beam(int r, int c, int dr, int dc, bool clear = true)
{
    if (clear) {
        // Clear the visited grid
        visited = vector<vector<int>>(rows, vector<int>(cols, 0));
    }

    int count = 0;
    while (r >= 0 && r < rows && c >= 0 && c < cols) {
        // Check if we have been to this tile coming from the same direction
        // In that case we have a loop and we are done
        int mask = ((dr < 0) ? 1 : ((dr > 0) ? 2 : 0)) | ((dc < 0) ? 4 : ((dc > 0) ? 8 : 0));
        if (visited[r][c] & mask) break;

        // Count the tile if this is the first time we entered it
        if (!visited[r][c]) count++;
        visited[r][c] |= mask;

        // Check the tile and calculate a new direction
        char tile = grid[r][c];
        if (tile == '/') {
            swap(dr, dc);
            dr = -dr;
            dc = -dc;
        } else if (tile == '\\') {
            swap(dr, dc);
        } else if (tile == '|' && dc) {
            // Split the beam
            count += run_beam(r, c, -dc, 0, false);
            swap(dr, dc);
        } else if (tile == '-' && dr) {
            // Split the beam
            count += run_beam(r, c, 0, -dr, false);
            swap(dr, dc);
        }

        // Move
        r += dr;
        c += dc;
    }

    return count;
}

int main()
{
    // Parse input data
    string line;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    rows = grid.size();
    cols = grid[0].size();

    // Part 1
    int part1 = run_beam(0, 0, 0, 1);

    // Part 2
    int part2 = 0;
    for (int r = 0; r < rows; r++) {
        part2 = max(part2, run_beam(r, 0, 0, 1));
        part2 = max(part2, run_beam(r, cols - 1, 0, -1));
    }
    for (int c = 0; c < cols; c++) {
        part2 = max(part2, run_beam(0, c, 1, 0));
        part2 = max(part2, run_beam(rows - 1, c, -1, 0));
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
