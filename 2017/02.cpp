/*
 * Problem 2, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
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
    vector<vector<int>> sheet;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& v = sheet.emplace_back();
        int num;
        while (ss >> num) {
            v.push_back(num);
        }
    }

    // Solve part 1
    int part1 = 0;
    {
        for (const auto& v : sheet) {
            CHECK(!v.empty());
            auto [min, max] = ranges::minmax_element(v);
            part1 += *max - *min;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        for (const auto& v : sheet) {
            int vsize = v.size();
            for (int i = 0; i < vsize; ++i) {
                for (int j = 0; j < vsize; ++j) {
                    if (i == j) continue;
                    if (v[j] && v[i] % v[j] == 0) part2 += v[i] / v[j];
                }
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
