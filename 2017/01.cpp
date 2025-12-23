/*
 * Problem 1, Advent of Code 2017
 * Danjel McGougan
 */

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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    string digits;
    CHECK(getline(in, digits));
    int dsize = digits.size();

    // Solve part 1
    int part1 = 0;
    {
        char d = digits[dsize - 1];
        for (int i = 0; i < dsize; ++i) {
            char next = digits[i];
            if (d == next) part1 += d - '0';
            d = next;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        for (int i = 0; i < dsize; ++i) {
            if (digits[i] == digits[(i + dsize / 2) % dsize]) part2 += digits[i] - '0';
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
