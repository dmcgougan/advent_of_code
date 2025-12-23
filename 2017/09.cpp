/*
 * Problem 9, Advent of Code 2017
 * Danjel McGougan
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <utility>

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
    string stream;
    CHECK(getline(in, stream) && !stream.empty());
    int ssize = stream.size();

    // Common
    int pos = 0;
    auto parse = [&](auto& self, int depth) -> pair<int, int> {
        int score = depth + 1;
        int gchars = 0;
        bool in_garbage = false;
        CHECK(pos < ssize && stream[pos++] == '{');
        for (;;) {
            CHECK(pos < ssize);
            char c = stream[pos];
            switch (c) {
            case '{':
                if (!in_garbage) {
                    auto [sc, gc] = self(self, depth + 1);
                    score += sc;
                    gchars += gc;
                } else {
                    ++gchars;
                }
                break;
            case '}':
                if (!in_garbage) {
                    CHECK(depth > 0 || pos + 1 == ssize);
                    return {score, gchars};
                }
                ++gchars;
                break;
            case '<':
                if (in_garbage) ++gchars;
                in_garbage = true;
                break;
            case '>':
                in_garbage = false;
                break;
            case '!':
                if (in_garbage) ++pos;
                break;
            default:
                if (in_garbage) ++gchars;
                break;
            }
            ++pos;
        }
    };

    // Solve both parts
    auto [part1, part2] = parse(parse, 0);

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
