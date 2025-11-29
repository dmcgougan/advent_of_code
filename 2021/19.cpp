/*
 * Problem 19, Advent of Code 2021
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;
using vec3 = array<int, 3>;

// Transform a coordinate based on a transformation index 0 to 47
// The index represents the possible rotations and facing directions in some order
// Not sure why there is double the number of combinations (handedness?)
static vec3 transform(const vec3& c, int ix)
{
    static const int p[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
    return {(ix & 1 ? -1 : 1) * c[p[ix >> 3][0]],
            (ix & 2 ? -1 : 1) * c[p[ix >> 3][1]],
            (ix & 4 ? -1 : 1) * c[p[ix >> 3][2]]};
}

// Transform all coordinates in the vector
static void transform(vector<vec3>& scanner, int ix)
{
    for (vec3& v : scanner) v = transform(v, ix);
}

// Translate all coordinates in the vector
static void translate(vector<vec3>& scanner, const vec3& location)
{
    for (vec3& v : scanner) {
        v[0] -= location[0];
        v[1] -= location[1];
        v[2] -= location[2];
    }
}

// Check if a pair of scanners see at least 12 common beacons and calculate transformation and
// translation to get scanner s1 in the same coordinate system as s0
// Returns the transformation index or -1 if at least 12 common beacons could not be found
static int check_pair(const vector<vec3>& s0, const vector<vec3>& s1, vec3& location)
{
    for (int ix = 0; ix < 48; ++ix) {
        map<vec3, int> translation_counts;
        vector<vec3> s1_t(s1);
        transform(s1_t, ix);
        for (const vec3& v0 : s0) {
            for (const vec3& v1 : s1_t) {
                vec3 t{v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
                if (++translation_counts[t] == 12) {
                    // Found at least 12 matching points
                    location = t;
                    return ix;
                }
            }
        }
    }
    return -1;
}

int main()
{
    // Parse input
    vector<vector<vec3>> scanners;
    string line;
    getline(cin, line);
    do {
        vector<vec3> scanner;
        while (getline(cin, line) && !line.empty()) {
            int x, y, z;
            assert(sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3);
            scanner.push_back({x, y, z});
        }
        scanners.push_back(move(scanner));
    } while (getline(cin, line));

    // Transform and translate all coordinates of all scanners to match with scanner 0
    vector<vec3> loc(scanners.size());
    vector<bool> done(scanners.size());
    vector<vector<int>> checked(scanners.size(), vector<int>(scanners.size(), false));
    done[0] = true;
    size_t done_count = 1;
    do {
        for (size_t i = 0; i < scanners.size(); ++i) {
            for (size_t j = 0; j < scanners.size(); ++j) {
                if (done[i] && !done[j] && !checked[i][j]) {
                    vec3 location;
                    int ix;
                    if ((ix = check_pair(scanners[i], scanners[j], location)) >= 0) {
                        transform(scanners[j], ix);
                        translate(scanners[j], location);
                        loc[j] = location;
                        done[j] = true;
                        done_count++;
                    }
                    checked[i][j] = true;
                }
            }
        }
    } while (done_count < scanners.size());

    // Get all beacons
    set<vec3> beacons;
    for (const auto& scanner : scanners) {
        for (const auto& v : scanner) {
            beacons.insert(v);
        }
    }
    int part1 = beacons.size();

    // Find maximum Manhattan distance between any pair of scanners
    int part2 = 0;
    for (size_t i = 0; i < loc.size(); ++i) {
        for (size_t j = i + 1; j < loc.size(); ++j) {
            part2 = max(part2,
                        abs(loc[i][0] - loc[j][0]) + abs(loc[i][1] - loc[j][1]) +
                            abs(loc[i][2] - loc[j][2]));
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
