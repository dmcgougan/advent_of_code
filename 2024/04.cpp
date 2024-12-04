/*
 * Problem 4, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int part1 = 0;
    int part2 = 0;

    // Read input
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        grid.emplace_back(line);
    }
    int rows = grid.size();
    int cols = grid[0].size();

    // Part 1
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            // Check if this pos could be the end of 'XMAS'
            if (grid[row][col] != 'S') continue;

            // Check all directions backwards to see how many 'XMAS' we find
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (!dr && !dc) continue;

                    // Find pos of 'X'
                    int r = row - 3 * dr;
                    int c = col - 3 * dc;

                    // Check if the 'X' would be outside of the grid
                    if (r < 0 || r >= rows || c < 0 || c >= cols) continue;

                    // Check if the letters are correct; 'S' was already checked
                    if (grid[r][c] == 'X' && grid[r + dr][c + dc] == 'M' &&
                        grid[r + 2 * dr][c + 2 * dc] == 'A')
                    {
                        part1++;
                    }
                }
            }
        }
    }

    // Part 2
    for (int r = 1; r < rows - 1; r++) {
        for (int c = 1; c < cols - 1; c++) {
            // Check if this pos could be the middle of the 'X-MAS' shape
            if (grid[r][c] != 'A') continue;
            if ((grid[r - 1][c - 1] != 'M' || grid[r + 1][c + 1] != 'S') &&
                (grid[r - 1][c - 1] != 'S' || grid[r + 1][c + 1] != 'M'))
            {
                continue;
            }
            if ((grid[r - 1][c + 1] != 'M' || grid[r + 1][c - 1] != 'S') &&
                (grid[r - 1][c + 1] != 'S' || grid[r + 1][c - 1] != 'M'))
            {
                continue;
            }
            part2++;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
