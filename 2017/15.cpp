/*
 * Problem 15, Advent of Code 2017
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
using ull = unsigned long long;

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
    ull init[2];
    {
        string line;
        size_t p;
        for (int i = 0; i < 2; ++i) {
            CHECK(getline(in, line));
            CHECK((p = line.find_last_not_of("0123456789")) != string::npos);
            init[i] = atoll(line.c_str() + p + 1);
        }
    }

    // Solve part 1
    int part1 = 0;
    {
        auto [a, b] = init;
        for (int i = 0; i < 40 * 1000 * 1000; ++i) {
            a = (a * 16807) % 2147483647;
            b = (b * 48271) % 2147483647;
            if ((a & 0xffff) == (b & 0xffff)) ++part1;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        auto [a, b] = init;
        for (int i = 0; i < 5 * 1000 * 1000; ++i) {
            do {
                a = (a * 16807) % 2147483647;
            } while (a & 3);
            do {
                b = (b * 48271) % 2147483647;
            } while (b & 7);
            if ((a & 0xffff) == (b & 0xffff)) ++part2;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
