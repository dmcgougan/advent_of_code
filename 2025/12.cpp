/*
 * Problem 12, Advent of Code 2025
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <source_location>
#include <sstream>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned int line)
{
    cerr << std::format("CHECK failed at line {}\n", line);
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
    constexpr int SHAPES = 6;
    constexpr int SIDE = 3;
    vector<vector<string>> shapes;
    vector<pair<array<int, 2>, array<int, SHAPES>>> regions;
    {
        string line;
        for (int i = 0; i < SHAPES; ++i) {
            CHECK(getline(in, line));
            auto& shape = shapes.emplace_back();
            for (int j = 0; j < SIDE; j++) {
                CHECK(getline(in, line));
                if (line.size() == SIDE) shape.emplace_back(std::move(line));
            }
            CHECK(shape.size() == SIDE);
            CHECK(getline(in, line) && line.empty());
        }
        while (getline(in, line)) {
            stringstream ss(line);
            auto& [size, counts] = regions.emplace_back();
            CHECK(ss >> size[0]);
            ss.ignore(1);
            CHECK(ss >> size[1]);
            ss.ignore(1);
            for (int i = 0; i < SHAPES; ++i) {
                CHECK(ss >> counts[i]);
            }
        }
    }

    // Get areas of each shape (how many tiles that are #)
    array<int, SHAPES> areas;
    for (int i = 0; i < SHAPES; ++i) {
        const vector<string>& shape = shapes[i];
        int area = 0;
        for (int y = 0; y < SIDE; ++y) {
            for (int x = 0; x < SIDE; ++x) {
                if (shape[y][x] == '#') ++area;
            }
        }
        areas[i] = area;
    }

    // This problem is basically an easter egg!
    // The regions have sizes that are either trivially possible or trivially impossible
    // There are no region sizes that are hard to solve
    int part1 = 0;
    for (const auto& [size, counts] : regions) {
        int region_area = size[0] * size[1];
        // Get a lower and upper bound on how much space the shapes will occupy.
        // The lower bound is perfect packing; the region is exactly the total number of #
        // The upper bound is sloppy packing; let each shape occupy a 3x3 area (trivially possible)
        int shapes_area_lb = 0;
        int shapes_area_ub = 0;
        for (int i = 0; i < SHAPES; ++i) {
            shapes_area_lb += counts[i] * areas[i];
            shapes_area_ub += counts[i] * SIDE * SIDE;
        }
        if (region_area >= shapes_area_ub) {
            // It is trivially possible to fit the shapes
            ++part1;
        } else {
            // Check that this one is impossible using the lower bound
            CHECK(region_area < shapes_area_lb);
        }
    }

    // Output answers
    cout << std::format("Part 1: {}\n", part1);
    cout << std::format("Part 2: <click>\n");

    return 0;
}
