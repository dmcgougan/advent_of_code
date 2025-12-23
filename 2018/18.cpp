/*
 * Problem 18, Advent of Code 2018
 * Danjel McGougan
 */

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <unordered_map>
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
    vector<string> grid;
    for (string line; getline(in, line);) {
        CHECK(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    CHECK(!grid.empty() && grid[0].size());
    int rows = grid.size();
    int cols = grid[0].size();

    // Common
    auto step = [&](const vector<string>& grid, vector<string>& next) -> int {
        int tot_trees = 0;
        int tot_yards = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int trees = 0;
                int yards = 0;
                static const int D[][2] =
                    {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
                for (auto [dr, dc] : D) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    trees += (grid[nr][nc] == '|');
                    yards += (grid[nr][nc] == '#');
                }
                char g = grid[r][c];
                char n = g;
                if (g == '.') {
                    if (trees >= 3) n = '|';
                } else if (g == '|') {
                    if (yards >= 3) n = '#';
                } else if (!yards || !trees) {
                    n = '.';
                }
                next[r][c] = n;
                tot_trees += (n == '|');
                tot_yards += (n == '#');
            }
        }
        return tot_trees * tot_yards;
    };

    // Solve part 1
    int part1 = 0;
    {
        vector<string> g0 = grid;
        vector<string> g1(rows, string(cols, '.'));
        vector<string>* cur = &g0;
        vector<string>* next = &g1;
        for (int i = 0; i < 10; ++i) {
            part1 = step(*cur, *next);
            swap(cur, next);
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Hash the contents of the grid
        auto grid_hash = [](const vector<string>& grid) -> uint64_t {
            uint64_t h = 0;
            for (const string& row : grid) {
                h ^= hash<string>{}(row) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
            }
            return h;
        };
        vector<string> g0 = grid;
        vector<string> g1(rows, string(cols, '.'));
        vector<string>* cur = &g0;
        vector<string>* next = &g1;
        unordered_map<uint64_t, int> seen;
        bool jumped = false;
        seen[0] = grid_hash(*cur);
        for (int i = 1; i <= 1000000000; ++i) {
            part2 = step(*cur, *next);
            swap(cur, next);
            if (jumped) continue;
            uint64_t h = grid_hash(*cur);
            auto it = seen.find(h);
            if (it == seen.end()) {
                seen[h] = i;
            } else {
                // We have seen this state before; jump ahead
                int steps = i - it->second;
                i += (1000000000 - i) / steps * steps;
                jumped = true;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
