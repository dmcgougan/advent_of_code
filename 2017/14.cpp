/*
 * Problem 14, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
#include <queue>
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

// Knot hash from day 10
using hash_t = array<uint8_t, 16>;
static hash_t knot_hash(const string& data)
{
    // Create the list with 0 to 255
    array<uint8_t, 256> list;
    ranges::iota(list, 0);

    // Helper
    unsigned pos = 0;
    unsigned skip = 0;
    auto do_len = [&](char len_char) {
        unsigned len = static_cast<unsigned char>(len_char);
        // Reverse
        for (unsigned i = 0; i < len / 2; ++i) {
            swap(list[(pos + i) & 0xff], list[(pos + len - 1 - i) & 0xff]);
        }
        // Move
        pos = (pos + len + skip) & 0xff;
        // Increase skip size
        ++skip;
    };

    // Run the 64 rounds
    for (int i = 0; i < 64; ++i) {
        for (char len_char : data) {
            do_len(len_char);
        }
        static const char suffix[] = {17, 31, 73, 47, 23};
        for (char len_char : suffix) {
            do_len(len_char);
        }
    }

    // Get the hash
    hash_t hash;
    for (int i = 0; i < 16; ++i) {
        hash[i] = 0;
        for (int j = 0; j < 16; ++j) {
            hash[i] ^= list[16 * i + j];
        }
    }

    return hash;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    string key;
    CHECK(getline(in, key));

    // Common
    array<hash_t, 128> grid;
    for (size_t i = 0; i < grid.size(); ++i) {
        grid[i] = knot_hash(format("{}-{}", key, i));
    }

    // Solve part 1
    int part1 = 0;
    {
        for (const auto& hash : grid) {
            // Get number of 1-bits
            for (auto bits : hash) part1 += popcount(bits);
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Check if a specific bit in the grid is set
        auto is_used = [&](int row, int col) {
            return !!(grid[row][col >> 3] & (1u << (~col & 7)));
        };
        array<array<bool, 128>, 128> visited{};
        queue<array<int, 2>> q;
        for (int r = 0; r < 128; ++r) {
            for (int c = 0; c < 128; ++c) {
                if (visited[r][c] || !is_used(r, c)) continue;
                // Found a new region; traverse it using BFS and mark the squares as visited
                ++part2;
                q.push({r, c});
                visited[r][c] = true;
                while (!q.empty()) {
                    auto [rr, cc] = q.front();
                    q.pop();
                    static const int dir[][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
                    for (auto [dr, dc] : dir) {
                        int nr = rr + dr;
                        int nc = cc + dc;
                        if (nr < 0 || nr >= 128 || nc < 0 || nc >= 128) continue;
                        if (visited[nr][nc] || !is_used(nr, nc)) continue;
                        q.push({nr, nc});
                        visited[nr][nc] = true;
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
