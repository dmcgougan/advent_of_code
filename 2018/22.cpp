/*
 * Problem 22, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    int depth;
    int tx, ty;
    {
        string line;
        CHECK(getline(in, line));
        CHECK(sscanf(line.c_str(), "depth: %d", &depth) == 1);
        CHECK(getline(in, line));
        CHECK(sscanf(line.c_str(), "target: %d,%d", &tx, &ty) == 2);
    }

    // Common
    const int xsize = max(tx + tx / 2, 400);
    const int ysize = max(ty + ty / 2, 400);
    vector<int> cache(xsize * ysize, -1);
    auto erosion = [&](const auto& gindex, int x, int y) {
        int& c = cache[y * xsize + x];
        if (c >= 0) return c;
        int erosion = (gindex(gindex, x, y) + depth) % 20183;
        c = erosion;
        return erosion;
    };
    auto gindex = [&](const auto& gindex, int x, int y) {
        if (x == tx && y == ty) return 0;
        if (y == 0) {
            return x * 16807;
        }
        if (x == 0) {
            return y * 48271;
        }
        return erosion(gindex, x - 1, y) * erosion(gindex, x, y - 1);
    };
    auto type = [&](int x, int y) { return erosion(gindex, x, y) % 3; };

    // Solve part 1
    int part1 = 0;
    {
        for (int y = 0; y <= ty; ++y) {
            for (int x = 0; x <= tx; ++x) {
                part1 += type(x, y);
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        enum
        {
            NEITHER = 0,
            TORCH = 1,
            GEAR = 2,
        };

        // A* to find cheapest route
        auto h = [&](int x, int y, int equip) {
            // Admissible heuristic is manhattan distance + equipping the torch
            return abs(x - tx) + abs(y - ty) + (equip != TORCH ? 7 : 0);
        };
        vector<int> min_cost_v(xsize * ysize * 3, INT_MAX);
        auto min_cost = [&](int x, int y, int equip) -> int& {
            return min_cost_v[3 * xsize * y + 3 * x + equip];
        };
        priority_queue<array<int, 5>, vector<array<int, 5>>, greater<array<int, 5>>> q;
        min_cost(0, 0, TORCH) = 0;
        q.push({h(0, 0, TORCH), 0, 0, 0, TORCH});
        while (!q.empty()) {
            auto [f, cost, x, y, equip] = q.top();
            q.pop();

            // Check if we reached the goal state
            if (x == tx && y == ty && equip == TORCH) {
                part2 = cost;
                break;
            }

            // Discard stale states
            if (cost > min_cost(x, y, equip)) continue;

            // Move
            static const int D[][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
            for (auto [dx, dy] : D) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || ny < 0) continue;
                CHECK(nx < xsize && ny < ysize);
                int t = type(nx, ny);
                if (t == 0 && equip == NEITHER) continue;
                if (t == 1 && equip == TORCH) continue;
                if (t == 2 && equip == GEAR) continue;
                int& min_c = min_cost(nx, ny, equip);
                int nc = cost + 1;
                if (nc >= min_c) continue;
                min_c = nc;
                q.push({nc + h(nx, ny, equip), nc, nx, ny, equip});
            }

            // Change equipment
            for (int ne = 0; ne < 3; ++ne) {
                if (ne == equip) continue;
                int t = type(x, y);
                if (t == 0 && ne == NEITHER) continue;
                if (t == 1 && ne == TORCH) continue;
                if (t == 2 && ne == GEAR) continue;
                int& min_c = min_cost(x, y, ne);
                int nc = cost + 7;
                if (nc >= min_c) continue;
                min_c = nc;
                q.push({nc + h(x, y, ne), nc, x, y, ne});
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
