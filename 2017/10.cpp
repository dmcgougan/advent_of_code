/*
 * Problem 10, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
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
    string lengths_dec;
    string lengths_ascii;
    {
        CHECK(getline(in, lengths_ascii));
        stringstream ss(lengths_ascii);
        for (int len; ss >> len;) {
            CHECK(len >= 0 && len <= 255);
            lengths_dec.push_back(static_cast<char>(len));
            ss.ignore(1);
        }
        CHECK(!lengths_dec.empty());
        lengths_ascii += "\x11\x1f\x49\x2f\x17";  // 17, 31, 73, 47, 23
    }

    // Common
    auto round = [](const string& lengths, vector<int>& list, int& pos, int& skip) {
        for (char len_char : lengths) {
            int len = static_cast<unsigned char>(len_char);
            // Reverse
            for (int i = 0; i < len / 2; ++i) {
                swap(list[(pos + i) & 0xff], list[(pos + len - 1 - i) & 0xff]);
            }
            // Move
            pos = (pos + len + skip) & 0xff;
            // Increase skip size
            ++skip;
        }
    };

    // Solve part 1
    int part1 = 0;
    {
        // Create the list with 0 to 255
        vector<int> list(256);
        ranges::iota(list, 0);

        // Run the operations
        int pos = 0;
        int skip = 0;
        round(lengths_dec, list, pos, skip);

        // Get answer
        part1 = list[0] * list[1];
    }

    // Solve part 2
    string part2;
    {
        // Create the list with 0 to 255
        vector<int> list(256);
        ranges::iota(list, 0);

        // Run the 64 rounds
        int pos = 0;
        int skip = 0;
        for (int i = 0; i < 64; ++i) round(lengths_ascii, list, pos, skip);

        // Convert to dense hash and get hex values of it
        for (int i = 0; i < 256; i += 16) {
            int h = 0;
            for (int j = 0; j < 16; ++j) {
                h ^= list[i + j];
            }
            CHECK(h >= 0 && h <= 255);
            static const char hex_digit[] = "0123456789abcdef";
            part2 += hex_digit[h >> 4];
            part2 += hex_digit[h & 15];
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
