/*
 * Problem 7, Advent of Code 2025
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using ll = long long;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<string> grid;
    for (string line; getline(in, line);) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    assert(!grid.empty() && !grid[0].empty());
    int rows = grid.size();
    int cols = grid[0].size();
    int start = grid[0].find('S');

    // Solve part 1
    int part1 = 0;
    {
        vector<string> g = grid;
        auto beam = [&](auto self, int row, int col) -> void {
            while (row < rows && col >= 0 && col < cols) {
                if (g[row][col] == '.') {
                    g[row][col] = '|';
                    ++row;
                    continue;
                }
                if (g[row][col] == '|') break;
                if (g[row][col] == '^') {
                    self(self, row, col - 1);
                    self(self, row, col + 1);
                    ++part1;
                    break;
                }
            }
        };
        beam(beam, 1, start);
    }

    // Solve part 2
    ll part2 = 0;
    {
        map<array<int, 2>, ll> cache;
        vector<string> g = grid;
        auto beam = [&](auto self, int row, int col) -> ll {
            // The same position always gives the same answer
            // Check if the result is in the cache (memoization)
            auto it = cache.find({row, col});
            if (it != cache.end()) return it->second;
            int r = row;
            int c = col;
            ll result = 1;
            while (r < rows && c < cols && c >= 0) {
                if (g[r][c] == '.') {
                    ++r;
                    continue;
                }
                if (g[r][c] == '^') {
                    // Sum of left and right sides
                    result = self(self, r, c - 1) + self(self, r, c + 1);
                    break;
                }
            }
            cache[{row, col}] = result;
            return result;
        };
        part2 = beam(beam, 1, start);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
