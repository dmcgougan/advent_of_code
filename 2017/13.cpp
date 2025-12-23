/*
 * Problem 13, Advent of Code 2017
 * Danjel McGougan
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <numeric>

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
    vector<array<int, 2>> scanners;
    for (string line; getline(in, line);) {
        auto& s = scanners.emplace_back();
        CHECK(sscanf(line.c_str(), "%d: %d", &s[0], &s[1]) == 2);
    }

    // Solve part 1
    int part1 = 0;
    {
        for (auto [depth, range] : scanners) {
            if (depth % (2 * (range - 1)) == 0) part1 += depth * range;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        int delay = 0;
        for (;;) {
            bool caught = false;
            for (auto [depth, range] : scanners) {
                if ((depth + delay) % (2 * (range - 1)) == 0) {
                    caught = true;
                    break;
                }
            }
            if (!caught) break;
            ++delay;
        }
        part2 = delay;
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
