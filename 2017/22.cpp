/*
 * Problem 22, Advent of Code 2017
 * Danjel McGougan
 */

#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <unordered_map>
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

struct pair_hash_t
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const noexcept
    {
        size_t h1 = std::hash<T1>{}(p.first);
        size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ull + (h1 << 6) + (h1 >> 2));
    }
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    unordered_map<pair<int, int>, char, pair_hash_t> grid_init;
    int crow_init;
    int ccol_init;
    {
        string line;
        int r = 0;
        int cols = -1;
        for (; getline(in, line); ++r) {
            if (cols >= 0) CHECK(cols == int(line.size()));
            cols = line.size();
            for (int c = 0; c < cols; ++c) {
                if (line[c] == '#') grid_init[{r, c}] = '#';
            }
        }
        CHECK(r > 0 && cols > 0);
        crow_init = r / 2;
        ccol_init = cols / 2;
    }

    // Common
    constexpr int dir[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // Solve part 1
    int part1 = 0;
    {
        unordered_map<pair<int, int>, char, pair_hash_t> grid = grid_init;
        int crow = crow_init;
        int ccol = ccol_init;
        int cdir = 0;  // up
        for (int i = 0; i < 10'000; ++i) {
            char& node = grid[{crow, ccol}];
            if (node == '#') {
                // Infected; turn right
                cdir = (cdir + 1) & 3;
                node = 0;
            } else {
                CHECK(node == 0);
                // Clean; turn left
                cdir = (cdir + 3) & 3;
                node = '#';
                ++part1;
            }
            // Move forward
            crow += dir[cdir][0];
            ccol += dir[cdir][1];
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        unordered_map<pair<int, int>, char, pair_hash_t> grid = grid_init;
        int crow = crow_init;
        int ccol = ccol_init;
        int cdir = 0;  // up
        for (int i = 0; i < 10'000'000; ++i) {
            char& node = grid[{crow, ccol}];
            switch (node) {
            case 0:
                // Clean; turn left
                cdir = (cdir + 3) & 3;
                node = 'W';
                break;
            case 'W':
                // Weakened; do not turn
                node = '#';
                ++part2;
                break;
            case '#':
                // Infected; turn right
                cdir = (cdir + 1) & 3;
                node = 'F';
                break;
            case 'F':
                // Flagged; reverse direction
                cdir = (cdir + 2) & 3;
                node = 0;
                break;
            default:
                CHECK(false);
            }
            // Move forward
            crow += dir[cdir][0];
            ccol += dir[cdir][1];
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
