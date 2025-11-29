/*
 * Problem 12, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

using namespace std;

// Simulate one time step
static void step(vector<array<int, 6>>& moons)
{
    for (size_t i = 0; i < moons.size(); ++i) {
        // Gravity
        for (size_t j = i + 1; j < moons.size(); ++j) {
            for (size_t a = 0; a < 3; ++a) {
                int p0 = moons[i][a];
                int p1 = moons[j][a];
                // Update velocities based on their relative positions
                if (p0 == p1) continue;
                moons[i][a + 3] += (p1 > p0) ? 1 : -1;
                moons[j][a + 3] += (p1 > p0) ? -1 : 1;
            }
        }
        // Velocity
        moons[i][0] += moons[i][3];
        moons[i][1] += moons[i][4];
        moons[i][2] += moons[i][5];
    }
}

// Calculate a hash value of the state of the specified axis
static size_t get_state_hash(const vector<array<int, 6>>& moons, int axis)
{
    size_t h = 0;
    for (const auto& m : moons) {
        // Hash combine from boost
        h ^= hash<int>{}(m[axis]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= hash<int>{}(m[axis + 3]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

int main()
{
    // Parse input
    vector<array<int, 6>> moons;
    for (string line; getline(cin, line);) {
        int x, y, z;
        assert(sscanf(line.c_str(), "<x=%d, y=%d, z=%d>", &x, &y, &z) == 3);
        moons.push_back({x, y, z, 0, 0, 0});
    }
    assert(!moons.empty());

    // Solve part 1
    int part1 = 0;
    {
        vector<array<int, 6>> moons_p1(moons);
        for (int i = 0; i < 1000; ++i) step(moons_p1);
        for (const auto& m : moons_p1) {
            int pot = abs(m[0]) + abs(m[1]) + abs(m[2]);
            int kin = abs(m[3]) + abs(m[4]) + abs(m[5]);
            part1 += pot * kin;
        }
    }

    // Solve part 2
    int64_t part2 = 0;
    {
        // Since the axis are independent we can find cycles for each axis
        // separately and then deduce when all axis repeat together
        array<int, 3> cycle;
        for (size_t a = 0; a < 3; ++a) {
            vector<array<int, 6>> moons_p2(moons);
            unordered_map<size_t, int> states;
            states[get_state_hash(moons_p2, a)] = 0;
            for (int s = 1;; ++s) {
                step(moons_p2);
                size_t hash = get_state_hash(moons_p2, a);
                auto iter = states.find(hash);
                if (iter != states.end()) {
                    // Assume the cycle goes back to the initial state
                    // Otherwise we would have to dabble with solving a system of congruences
                    assert(iter->second == 0);
                    cycle[a] = s;
                    break;
                }
                states[hash] = s;
            }
        }
        // Calculate the Least Common Multiple of the three cycles
        int64_t lcm01 = int64_t(cycle[0]) * cycle[1] / gcd(cycle[0], cycle[1]);
        part2 = lcm01 * cycle[2] / gcd(lcm01, cycle[2]);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
