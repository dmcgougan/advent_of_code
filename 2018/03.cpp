/*
 * Problem 3, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Hash function for array
template <typename T, size_t N>
struct std::hash<array<T, N>>
{
    size_t operator()(const array<T, N>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 5>> claims;
    for (string line; getline(in, line);) {
        auto& a = claims.emplace_back();
        assert(sscanf(line.c_str(), "#%d @ %d,%d: %dx%d", &a[0], &a[1], &a[2], &a[3], &a[4]) == 5);
    }

    // Common data
    unordered_map<array<int, 2>, int> count;
    for (const auto& [id, col, row, width, height] : claims) {
        for (int c = col; c < col + width; ++c) {
            for (int r = row; r < row + height; ++r) {
                ++count[{c, r}];
            }
        }
    }

    // Solve part 1
    int part1 = 0;
    for (auto [_, c] : count) {
        if (c >= 2) ++part1;
    }

    // Solve part 2
    int part2 = 0;
    for (const auto& [id, col, row, width, height] : claims) {
        bool found = true;
        for (int c = col; c < col + width && found; ++c) {
            for (int r = row; r < row + height; ++r) {
                if (count[{c, r}] > 1) {
                    found = false;
                    break;
                }
            }
        }
        if (found) {
            part2 = id;
            break;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
