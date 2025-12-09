/*
 * Problem 9, Advent of Code 2025
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using ll = long long;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 2>> tile;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& pos = tile.emplace_back();
        ss >> pos[0];
        ss.ignore(1);
        ss >> pos[1];
    }
    int tiles = tile.size();

    // Solve part 1
    ll part1 = 0;
    {
        for (int i = 0; i < tiles; ++i) {
            for (int j = i + 1; j < tiles; ++j) {
                ll area = ll(abs(tile[i][0] - tile[j][0]) + 1) * (abs(tile[i][1] - tile[j][1]) + 1);
                part1 = max(part1, area);
            }
        }
    }

    // Solve part 2
    ll part2 = 0;
    {
        // Check if a rectangle with corners c0 and c1 is inside the bounding polygon.
        // I suspect there exists an easier solution to this part. The function here
        // is pretty general and handles any rectangle, but the rectangles in the problem
        // are special because they have two of their corners _on_ the bounding polygon.
        // There is probably a way to take advantage of that fact and make the code simpler.
        auto rectangle_is_inside = [&](const array<int, 2>& c0, const array<int, 2>& c1) -> bool {
            int crossings = 15;
            int boundary = 0;

            // Rectangle vertices
            int rx_min = min(c0[0], c1[0]);
            int rx_max = max(c0[0], c1[0]);
            int ry_min = min(c0[1], c1[1]);
            int ry_max = max(c0[1], c1[1]);
            array<int, 2> r0{rx_min, ry_min};
            array<int, 2> r1{rx_max, ry_min};
            array<int, 2> r2{rx_max, ry_max};
            array<int, 2> r3{rx_min, ry_max};

            // Walk the bounding polygon and check if the rectangle vertices are inside it
            array<int, 2> p0 = tile[tiles - 1];
            for (int i = 0; i < tiles; ++i) {
                array<int, 2> p1 = tile[i];
                if (p0[0] == p1[0]) {
                    // Found a vertical edge of the bounding polygon
                    int px = p0[0];
                    int py_min = min(p0[1], p1[1]);
                    int py_max = max(p0[1], p1[1]);

                    // Odd-even check (ray casting)
                    if (r0[0] < px && r0[1] >= py_min && r0[1] < py_max) crossings ^= 1;
                    if (r1[0] < px && r1[1] >= py_min && r1[1] < py_max) crossings ^= 2;
                    if (r2[0] < px && r2[1] >= py_min && r2[1] < py_max) crossings ^= 4;
                    if (r3[0] < px && r3[1] >= py_min && r3[1] < py_max) crossings ^= 8;

                    // Check if rectangle vertices are on the boundary of the polygon
                    if (r0[0] == px && r0[1] >= py_min && r0[1] <= py_max) boundary |= 1;
                    if (r1[0] == px && r1[1] >= py_min && r1[1] <= py_max) boundary |= 2;
                    if (r2[0] == px && r2[1] >= py_min && r2[1] <= py_max) boundary |= 4;
                    if (r3[0] == px && r3[1] >= py_min && r3[1] <= py_max) boundary |= 8;
                } else {
                    assert(p0[1] == p1[1]);

                    // Found a horizontal edge of the bounding polygon
                    int py = p0[1];
                    int px_min = min(p0[0], p1[0]);
                    int px_max = max(p0[0], p1[0]);

                    // Check if rectangle vertices are on the boundary of the polygon
                    if (r0[1] == py && r0[0] >= px_min && r0[0] <= px_max) boundary |= 1;
                    if (r1[1] == py && r1[0] >= px_min && r1[0] <= px_max) boundary |= 2;
                    if (r2[1] == py && r2[0] >= px_min && r2[0] <= px_max) boundary |= 4;
                    if (r3[1] == py && r3[0] >= px_min && r3[0] <= px_max) boundary |= 8;
                }
                p0 = p1;
            }
            if (crossings & ~boundary) {
                // One or more of the rectangle vertices are outside the bounding polygon
                return false;
            }

            // Now we need to check if any of the rectangle edges cross an edge of the polygon
            // Only true crosses need to be checked (crossings inside the boundaries)
            p0 = tile[tiles - 1];
            for (int i = 0; i < tiles; ++i) {
                array<int, 2> p1 = tile[i];
                if (p0[0] == p1[0]) {
                    // Vertical edge of the bounding polygon
                    int px = p0[0];
                    int py_min = min(p0[1], p1[1]);
                    int py_max = max(p0[1], p1[1]);
                    if (px > rx_min && px < rx_max && max(ry_min, py_min) < min(ry_max, py_max)) {
                        // Found a crossing; parts of the rectangle are outside the polygon
                        return false;
                    }
                } else {
                    // Horizontal edge of the bounding polygon
                    int py = p0[1];
                    int px_min = min(p0[0], p1[0]);
                    int px_max = max(p0[0], p1[0]);
                    if (py > ry_min && py < ry_max && max(rx_min, px_min) < min(rx_max, px_max)) {
                        // Found a crossing; parts of the rectangle are outside the polygon
                        return false;
                    }
                }
                p0 = p1;
            }

            return true;
        };

        // Find largest area of the rectangles that are inside the bounding polygon
        for (int i = 0; i < tiles; ++i) {
            for (int j = i + 1; j < tiles; ++j) {
                if (!rectangle_is_inside(tile[i], tile[j])) continue;
                ll area = ll(abs(tile[i][0] - tile[j][0]) + 1) * (abs(tile[i][1] - tile[j][1]) + 1);
                part2 = max(part2, area);
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
