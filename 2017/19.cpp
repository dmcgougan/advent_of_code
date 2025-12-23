/*
 * Problem 19, Advent of Code 2017
 * Danjel McGougan
 */

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, unsigned line = source_location::current().line())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(line);
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<string> grid;
    for (string line; getline(in, line);) {
        CHECK(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    CHECK(!grid.empty());
    int rows = grid.size();
    int cols = grid[0].size();
    int start_col;
    {
        size_t scol;
        CHECK((scol = grid[0].find_first_of('|')) != string::npos);
        start_col = scol;
    }

    // Solves part 1 and 2
    string part1;
    int part2 = 0;
    {
        int row = 0;
        int col = start_col;
        ++part2;
        int dr = 1;
        int dc = 0;
        for (;;) {
            row += dr;
            col += dc;
            CHECK(row >= 0 && row < rows && col >= 0 && col < cols);
            char tile = grid[row][col];
            if (tile == ' ') break;
            ++part2;
            if (isalpha(tile)) {
                part1 += tile;
            } else if (tile == '+') {
                // Time to turn
                swap(dr, dc);
                int nr = row + dr;
                int nc = col + dc;
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != ' ') {
                    continue;
                }
                dr = -dr;
                dc = -dc;
                nr = row + dr;
                nc = col + dc;
                CHECK(nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != ' ');
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
