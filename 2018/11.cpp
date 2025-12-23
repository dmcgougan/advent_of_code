/*
 * Problem 11, Advent of Code 2018
 * Danjel McGougan
 */

#include <climits>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned int line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, source_location loc = source_location::current())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(loc.line());
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    int serial;
    CHECK(in >> serial);

    // Common
    constexpr int X_SIZE = 300;
    constexpr int Y_SIZE = 300;

    // Convert coordinates to power (with x and y zero-based)
    auto power = [&](int x, int y) -> int {
        return ((x + 11) * (y + 1) + serial) * (x + 11) / 100 % 10 - 5;
    };

    // Precompute a 2D prefix sum array (zero-based)
    vector<vector<int>> psum(Y_SIZE + 1, vector<int>(X_SIZE + 1));
    for (int y = 0; y < Y_SIZE; ++y) {
        for (int x = 0; x < X_SIZE; ++x) {
            psum[y + 1][x + 1] = power(x, y) + psum[y][x + 1] + psum[y + 1][x] - psum[y][x];
        }
    }

    // Get the sum of all points in a square
    auto sqsum = [&](int x, int y, int sqsize) -> int {
        int xx = x + sqsize;
        int yy = y + sqsize;
        return psum[y][x] + psum[yy][xx] - psum[y][xx] - psum[yy][x];
    };

    // Solve part 1
    string part1;
    {
        int max_power = INT_MIN;
        for (int x = 0; x + 3 <= X_SIZE; ++x) {
            for (int y = 0; y + 3 <= Y_SIZE; ++y) {
                int p = sqsum(x, y, 3);
                if (p > max_power) {
                    max_power = p;
                    part1 = to_string(x + 1) + "," + to_string(y + 1);
                }
            }
        }
    }

    // Solve part 2
    string part2;
    {
        int max_power = INT_MIN;
        for (int x = 0; x < X_SIZE; ++x) {
            for (int y = 0; y < Y_SIZE; ++y) {
                int max_size = min(X_SIZE - x, Y_SIZE - y);
                for (int size = 1; size <= max_size; ++size) {
                    int p = sqsum(x, y, size);
                    if (p > max_power) {
                        max_power = p;
                        part2 = to_string(x + 1) + "," + to_string(y + 1) + "," + to_string(size);
                    }
                }
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
