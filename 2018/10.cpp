/*
 * Problem 10, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <print>
#include <set>
#include <source_location>
#include <vector>

using namespace std;
using ll = long long;

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
    vector<array<int, 4>> stars;
    {
        for (string line; getline(in, line);) {
            int x, y, dx, dy;
            CHECK(
                sscanf(line.c_str(), "position=<%d, %d> velocity=<%d,%d>", &x, &y, &dx, &dy) ==
                4);
            stars.push_back({x, y, dx, dy});
        }
    }
    CHECK(!stars.empty());

    // Solve parts 1 and 2

    // Find when the bounding box has a minimum area
    auto box_area = [&](int step, int& min_x, int& max_x, int& min_y, int& max_y) -> ll {
        min_x = INT_MAX;
        max_x = INT_MIN;
        min_y = INT_MAX;
        max_y = INT_MIN;
        for (auto [x, y, dx, dy] : stars) {
            int nx = x + step * dx;
            int ny = y + step * dy;
            min_x = min(min_x, nx);
            max_x = max(max_x, nx);
            min_y = min(min_y, ny);
            max_y = max(max_y, ny);
        }
        return static_cast<ll>(max_x - min_x) * (max_y - min_y);
    };
    int min_x, max_x, min_y, max_y;
    int step = 0;
    ll last_area = LLONG_MAX;
    for (;;) {
        ll area = box_area(step, min_x, max_x, min_y, max_y);
        if (area > last_area) {
            --step;
            break;
        }
        last_area = area;
        ++step;
    }

    // Print the text
    box_area(step, min_x, max_x, min_y, max_y);
    set<array<int, 2>> text;
    for (auto [x, y, dx, dy] : stars) {
        text.insert({x + step * dx, y + step * dy});
    }
    println("Part 1: ");
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            putchar(text.count({x, y}) ? '#' : '.');
        }
        println();
    }
    println("Part 2: {}", step);

    return 0;
}
