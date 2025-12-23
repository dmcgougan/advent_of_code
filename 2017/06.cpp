/*
 * Problem 6, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <set>
#include <source_location>
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
    vector<int> banks_init;
    for (int blocks; in >> blocks;) {
        banks_init.push_back(blocks);
    }
    CHECK(!banks_init.empty());

    // Common
    auto redist = [](vector<int>& banks) {
        size_t i = distance(banks.begin(), ranges::max_element(banks));
        int blocks = banks[i];
        banks[i] = 0;
        while (blocks > 0) {
            i = (i + 1 < banks.size() ? i + 1 : 0);
            ++banks[i];
            --blocks;
        }
    };

    // Solve part 1
    int part1 = 0;
    {
        vector<int> banks = banks_init;
        set<vector<int>> seen;
        seen.insert(banks);
        for (;;) {
            redist(banks);
            ++part1;
            if (seen.contains(banks)) break;
            seen.insert(banks);
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        vector<int> banks = banks_init;
        map<vector<int>, int> seen;
        seen[banks] = 0;
        int steps = 0;
        for (;;) {
            redist(banks);
            ++steps;
            auto it = seen.find(banks);
            if (it != seen.end()) {
                part2 = steps - it->second;
                break;
            }
            seen[banks] = steps;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
