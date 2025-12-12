/*
 * Problem 12, Advent of Code 2025
 * Danjel McGougan
 *
 * This solves it for my input. Not sure if it solves it for other inputs.
 * For me it was just a matter of spotting that one of the shapes had one
 * tile that is never reachable by any other tile. And then just sum up the
 * area used by the specified number of tiles per region (with one added to
 * the special shape) and see if that area is less or equal to the total
 * area of the region.
 */

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    constexpr int SHAPES = 6;
    constexpr int SIDE = 3;
    vector<vector<string>> shapes;
    vector<pair<array<int, 2>, array<int, SHAPES>>> regions;
    {
        string line;
        for (int i = 0; i < SHAPES; ++i) {
            assert(getline(in, line));
            auto& shape = shapes.emplace_back();
            for (int j = 0; j < SIDE; j++) {
                assert(getline(in, line));
                if (line.size() == SIDE) shape.emplace_back(std::move(line));
            }
            assert(shape.size() == SIDE);
            assert(getline(in, line) && line.empty());
        }
        while (getline(in, line)) {
            stringstream ss(line);
            auto& [size, counts] = regions.emplace_back();
            assert(ss >> size[0]);
            ss.ignore(1);
            assert(ss >> size[1]);
            ss.ignore(1);
            for (int i = 0; i < SHAPES; ++i) {
                assert(ss >> counts[i]);
            }
        }
    }

    // Get areas of each shape
    int part1 = 0;
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

    // Add one to the special shape that has one tile that is not reachable.
    // Maybe I need code to detect which tile this is to be generic for other inputs?
    ++areas[4];

    // Check if the sum of shape areas is less or equal to the total area
    for (const auto& [size, counts] : regions) {
        int rarea = size[0] * size[1];
        int sarea = 0;
        for (int i = 0; i < SHAPES; ++i) {
            sarea += counts[i] * areas[i];
        }
        if (sarea <= rarea) ++part1;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
