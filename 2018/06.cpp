/*
 * Problem 6, Advent of Code 2018
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 2>> pos;
    for (int r, c; in >> r;) {
        assert(in.ignore(1) && in >> c);
        pos.push_back({r, c});
    }
    assert(!pos.empty());
    int positions = pos.size();

    // Common data
    int min_row = INT_MAX, max_row = INT_MIN;
    int min_col = INT_MAX, max_col = INT_MIN;
    for (auto [r, c] : pos) {
        min_row = min(min_row, r);
        max_row = max(max_row, r);
        min_col = min(min_col, c);
        max_col = max(max_col, c);
    }

    // Solve part 1
    int part1 = 0;
    {
        // This is not guaranteed to find an area that is finite
        // It worked for my input and I suspect it works for all inputs
        vector<int> area(positions);
        for (int r = min_row; r <= max_row; ++r) {
            for (int c = min_col; c <= max_col; ++c) {
                int min_dist = INT_MAX;
                int min_pos = 0;
                bool tied = false;
                for (int i = 0; i < positions; ++i) {
                    int dist = abs(pos[i][0] - r) + abs(pos[i][1] - c);
                    if (dist <= min_dist) {
                        if (dist == min_dist) tied = true;
                        if (dist < min_dist) tied = false;
                        min_dist = dist;
                        min_pos = i;
                    }
                }
                if (!tied) ++area[min_pos];
            }
        }
        part1 = *std::max_element(area.begin(), area.end());
    }

    // Solve part 2
    int part2 = 0;
    {
        for (int r = min_row; r <= max_row; ++r) {
            for (int c = min_col; c <= max_col; ++c) {
                int dist = 0;
                for (auto [rr, cc] : pos) {
                    dist += abs(rr - r) + abs(cc - c);
                }
                if (dist < 10000) ++part2;
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
