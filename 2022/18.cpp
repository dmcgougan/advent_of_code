/*
 * Problem 18, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const std::string& str, const std::string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    assert(!sep.empty());
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.emplace_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.emplace_back(move(s));
    }
    return result;
}

// Limits of the world cube
static ll min_x = INT64_MAX, max_x = INT64_MIN;
static ll min_y = INT64_MAX, max_y = INT64_MIN;
static ll min_z = INT64_MAX, max_z = INT64_MIN;

// Sets
static set<tuple<ll, ll, ll>> cubes;
static set<tuple<ll, ll, ll>> exterior_cubes;

// Check if a voxel is solid (contains a cube)
static bool is_solid(ll x, ll y, ll z) { return cubes.count(make_tuple(x, y, z)) != 0; }

// Check if a voxel is exterior (can be reached by water and steam)
static bool is_exterior(ll x, ll y, ll z)
{
    // Check if the voxel is either outside the world cube or in the
    // exterior_cubes set
    return x < min_x || x > max_x || y < min_y || y > max_y || z < min_z || z > max_z ||
        exterior_cubes.count(make_tuple(x, y, z)) != 0;
}

// Recursive depth-first search (DFS) to find all exterior voxels in the world
// cube
static void traverse(ll x, ll y, ll z)
{
    exterior_cubes.insert(make_tuple(x, y, z));
    if (x - 1 >= min_x && !is_solid(x - 1, y, z) && !exterior_cubes.count(make_tuple(x - 1, y, z)))
        traverse(x - 1, y, z);
    if (x + 1 <= max_x && !is_solid(x + 1, y, z) && !exterior_cubes.count(make_tuple(x + 1, y, z)))
        traverse(x + 1, y, z);
    if (y - 1 >= min_y && !is_solid(x, y - 1, z) && !exterior_cubes.count(make_tuple(x, y - 1, z)))
        traverse(x, y - 1, z);
    if (y + 1 <= max_y && !is_solid(x, y + 1, z) && !exterior_cubes.count(make_tuple(x, y + 1, z)))
        traverse(x, y + 1, z);
    if (z - 1 >= min_z && !is_solid(x, y, z - 1) && !exterior_cubes.count(make_tuple(x, y, z - 1)))
        traverse(x, y, z - 1);
    if (z + 1 <= max_z && !is_solid(x, y, z + 1) && !exterior_cubes.count(make_tuple(x, y, z + 1)))
        traverse(x, y, z + 1);
}

// Set up the DFS and execute
static void find_exterior()
{
    // Find an exterior voxel within the world cube
    // Go along an edge since those voxels must be exterior if they are not solid
    ll x;
    for (x = min_x; x <= max_x; ++x) {
        if (!is_solid(x, 0, 0)) break;
    }
    assert(x <= max_x);
    traverse(x, 0, 0);
}

int main()
{
    // Parse the input
    string line;
    while (getline(cin, line)) {
        auto nums = split(line, ",");
        assert(nums.size() == 3);
        ll x, y, z;
        stringstream(nums[0]) >> x;
        max_x = max(max_x, x);
        min_x = min(min_x, x);
        stringstream(nums[1]) >> y;
        max_y = max(max_y, y);
        min_y = min(min_y, y);
        stringstream(nums[2]) >> z;
        max_z = max(max_z, z);
        min_z = min(min_z, z);
        cubes.insert(make_tuple(x, y, z));
    }

    // Part 1 area
    ll p1_area = 0;
    for (auto [x, y, z] : cubes) {
        if (!is_solid(x + 1, y, z)) p1_area++;
        if (!is_solid(x - 1, y, z)) p1_area++;
        if (!is_solid(x, y + 1, z)) p1_area++;
        if (!is_solid(x, y - 1, z)) p1_area++;
        if (!is_solid(x, y, z + 1)) p1_area++;
        if (!is_solid(x, y, z - 1)) p1_area++;
    }

    // Traverse the world and find all exterior parts of it
    find_exterior();

    // Part 2 area
    ll p2_area = 0;
    for (auto [x, y, z] : cubes) {
        if (is_exterior(x + 1, y, z)) p2_area++;
        if (is_exterior(x - 1, y, z)) p2_area++;
        if (is_exterior(x, y + 1, z)) p2_area++;
        if (is_exterior(x, y - 1, z)) p2_area++;
        if (is_exterior(x, y, z + 1)) p2_area++;
        if (is_exterior(x, y, z - 1)) p2_area++;
    }

    // Output
    cout << "Part 1: " << p1_area << endl;
    cout << "Part 2: " << p2_area << endl;

    return 0;
}
