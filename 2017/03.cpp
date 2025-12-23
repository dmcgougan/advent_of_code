/*
 * Problem 3, Advent of Code 2017
 * Danjel McGougan
 */

#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
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
    int sq;
    CHECK(in >> sq);

    // Solve part 1
    int part1 = 0;
    {
        // Get how many turns of the spiral out we are
        int x;
        for (x = 1; x * x < sq; x += 2);
        // Calculate minimum distance to a mid-point
        int min_dist = INT_MAX;
        int p = x * x - x / 2;
        for (int i = 0; i < 4; ++i) {
            min_dist = min(min_dist, abs(sq - p));
            p -= x - 1;
        }
        // Steps to mid-point plus steps to center
        int steps = min_dist + x / 2;
        part1 += steps;
    }

    // Solve part 2
    int part2 = 0;
    {
        static const int dir[][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
        static const int dir_diag[][2] =
            {{0, 1}, {1, 1}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1}, {1, 0}, {-1, 1}};
        // Keep all numbers in a map; initialize it with the center 1
        int row = 0;
        int col = 0;
        map<pair<int, int>, int> nums;
        nums[{row, col}] = 1;
        // Generate the spiral coordinates
        bool done = false;
        for (int len = 1, d = 0; !done; ++len) {
            for (int rep = 0; rep < 2 && !done; ++rep) {
                auto [dr_sp, dc_sp] = dir[d++ & 3];
                for (int i = 0; i < len; ++i) {
                    row += dr_sp;
                    col += dc_sp;
                    // Find neighboring numbers
                    int sum = 0;
                    for (auto [dr, dc] : dir_diag) {
                        auto it = nums.find({row + dr, col + dc});
                        if (it != nums.end()) sum += it->second;
                    }
                    // Check if we found the solution
                    if (sum > sq) {
                        part2 = sum;
                        done = true;
                        break;
                    }
                    // Add the new number
                    nums[{row, col}] = sum;
                }
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
