/*
 * Problem 25, Advent of Code 2024
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    string line;
    vector<string> grid;
    vector<array<int, 5>> locks;
    vector<array<int, 5>> keys;
    for (;;) {
        grid.clear();
        while (getline(cin, line) && !line.empty()) {
            grid.emplace_back(move(line));
        }
        if (grid.empty()) break;
        assert(grid.size() == 7);
        bool is_lock = (grid[0][0] == '#');
        array<int, 5> heights;
        for (int col = 0; col < 5; ++col) {
            if (is_lock) {
                // Determine lock height for the current column
                for (int row = 5; row >= 0; --row) {
                    assert(grid[row].size() == 5);
                    if (grid[row][col] == '#') {
                        heights[col] = row;
                        break;
                    }
                }
            } else {
                // Determine key height for the current column
                for (int row = 1; row <= 6; ++row) {
                    assert(grid[row].size() == 5);
                    if (grid[row][col] == '#') {
                        heights[col] = 6 - row;
                        break;
                    }
                }
            }
        }
        (is_lock ? locks : keys).emplace_back(heights);
    }

    // Get number of key/lock pairs that match
    int part1 = 0;
    for (size_t i = 0; i < locks.size(); i++) {
        for (size_t j = 0; j < keys.size(); j++) {
            size_t k;
            for (k = 0; k < 5; k++) {
                if (locks[i][k] + keys[j][k] > 5) break;
            }
            part1 += (k == 5);
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
