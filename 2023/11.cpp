/*
 * Problem 11, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = int64_t;

int main()
{
    // Parse input data and find empty rows and columns
    vector<array<int, 2>> image;
    unordered_set<int> expanded_rows;
    unordered_set<int> expanded_cols;
    int rows = 0;
    int cols = 0;
    string line;
    getline(cin, line);
    cols = int(line.size());
    vector<bool> col_found(cols, false);
    do {
        bool row_found = false;
        for (int col = 0; col < cols; ++col) {
            if (line[col] == '#') {
                image.push_back({rows, col});
                row_found = true;
                col_found[col] = true;
            }
        }
        if (!row_found) expanded_rows.insert(rows);
        ++rows;
    } while (getline(cin, line));
    for (int col = 0; col < cols; ++col) {
        if (!col_found[col]) expanded_cols.insert(col);
    }

    // Find distances between pairs of galaxies
    ll part1 = 0;
    ll part2 = 0;
    for (size_t p1 = 0; p1 + 1 < image.size(); ++p1) {
        for (size_t p2 = p1 + 1; p2 < image.size(); ++p2) {
            auto [row1, col1] = image[p1];
            auto [row2, col2] = image[p2];
            // Count how many expanded rows and columns there are between the two galaxies
            int exp_rows = 0;
            int exp_cols = 0;
            for (int row = min(row1, row2) + 1; row < max(row1, row2); ++row) {
                if (expanded_rows.count(row)) ++exp_rows;
            }
            for (int col = min(col1, col2) + 1; col < max(col1, col2); ++col) {
                if (expanded_cols.count(col)) ++exp_cols;
            }
            // Calculate Manhattan distance and add the expanded rows and columns
            ll dist = abs(row1 - row2) + abs(col1 - col2);
            part1 += dist + exp_rows + exp_cols;
            part2 += dist + 999999 * (exp_rows + exp_cols);
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
