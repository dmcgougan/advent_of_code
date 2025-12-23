/*
 * Problem 17, Advent of Code 2017
 * Danjel McGougan
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
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
    int steps;
    CHECK((in >> steps) && steps > 0);

    // Solve part 1
    int part1;
    {
        vector<int> buf(1, 0);
        int p = 0;
        for (int v = 1; v <= 2017; ++v) {
            p = (p + steps) % buf.size() + 1;
            buf.insert(buf.begin() + p, v);
        }
        part1 = buf[(p + 1) % buf.size()];
    }

    // Solve part 2
    int part2 = 0;
    {
        int p = 0;
        int bsize = 1;
        for (int v = 1; v <= 50'000'000; ++v) {
            p = (p + steps) % bsize + 1;
            bsize++;
            if (p == 1) {
                // The element after 0 changed
                part2 = v;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
