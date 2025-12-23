/*
 * Problem 24, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<pair<int, int>> components;
    int max_pins = 0;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& [pins0, pins1] = components.emplace_back();
        CHECK((ss >> pins0) && pins0 >= 0);
        ss.ignore(1);
        CHECK((ss >> pins1) && pins1 >= 0);
        max_pins = max(max_pins, pins0);
        max_pins = max(max_pins, pins1);
    }
    int csize = components.size();
    CHECK(csize > 0 && csize <= 64);

    // Common; precompute mapping from pins to component
    vector<vector<int>> port(max_pins + 1);
    for (int i = 0; i < csize; ++i) {
        auto [pins0, pins1] = components[i];
        port[pins0].push_back(i);
        if (pins1 != pins0) port[pins1].push_back(i);
    }

    // DFS
    auto dfs = [&](auto& self, uint64_t used, int strength, int npins, const auto& best) -> void {
        best(used, strength);
        // Find all components that have matching pins
        for (int c : port[npins]) {
            if (used & (1ull << c)) continue;
            auto [pins0, pins1] = components[c];
            CHECK(pins0 == npins || pins1 == npins);
            self(self,
                 used | (1ull << c),
                 strength + pins0 + pins1,
                 npins == pins0 ? pins1 : pins0,
                 best);
        }
    };

    // Solve part 1
    int part1 = 0;
    {
        dfs(dfs, 0, 0, 0, [&](uint64_t used, int strength) { part1 = max(part1, strength); });
    }

    // Solve part 2
    int part2 = 0;
    {
        int max_len = 0;
        dfs(dfs, 0, 0, 0, [&](uint64_t used, int strength) {
            int len = popcount(used);
            if (len > max_len || (len == max_len && strength > part2)) {
                max_len = len;
                part2 = strength;
            }
        });
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
