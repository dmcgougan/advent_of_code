/*
 * Problem 11, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
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

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, p0 = 0;
    while ((p = s.find(sep, p0)) != string::npos) {
        if (p > p0) ret.emplace_back(s.substr(p0, p - p0));
        p0 = p + sep.size();
    }
    if (p0 < s.size()) ret.emplace_back(s.substr(p0));
    return ret;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<string> walk;
    {
        string line;
        CHECK(getline(in, line));
        walk = split(line, ",");
    }

    // Solve both parts
    int part1 = 0;
    int part2 = 0;
    {
        // Distance function for odd-q vertical layout:
        // Columns straight, rows staggered, odd columns "lower" than even columns
        auto hex_distance = [](int r0, int c0, int r1, int c1) {
            int dq = c0 - c1;
            int dr = r0 - r1 - (c0 - (c0 & 1)) / 2 + (c1 - (c1 & 1)) / 2;
            return max({abs(dq), abs(dr), abs(dq + dr)});
        };

        // Walk the grid and get distances
        int row = 0;
        int col = 0;
        for (const string& dir : walk) {
            if (dir == "n") {
                --row;
            } else if (dir == "s") {
                ++row;
            } else if (dir == "nw") {
                --col;
                if (col & 1) --row;
            } else if (dir == "ne") {
                ++col;
                if (col & 1) --row;
            } else if (dir == "sw") {
                if (col & 1) ++row;
                --col;
            } else {
                CHECK(dir == "se");
                if (col & 1) ++row;
                ++col;
            }
            part2 = max(part2, hex_distance(0, 0, row, col));
        }
        part1 = hex_distance(0, 0, row, col);
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
