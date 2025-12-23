/*
 * Problem 5, Advent of Code 2017
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
    vector<int> prg_init;
    for (int offset; in >> offset;) {
        prg_init.push_back(offset);
    }
    int psize = prg_init.size();
    CHECK(psize);

    // Solve part 1
    int part1 = 0;
    {
        vector<int> prg = prg_init;
        for (int pc = 0; pc >= 0 && pc < psize; ++part1) {
            pc += prg[pc]++;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        vector<int> prg = prg_init;
        for (int pc = 0; pc >= 0 && pc < psize; ++part2) {
            int offset = prg[pc];
            prg[pc] += (offset < 3 ? 1 : -1);
            pc += offset;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
