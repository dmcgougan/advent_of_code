/*
 * Problem 3, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Hash function for array
template <typename T, size_t size>
struct std::hash<array<T, size>>
{
    size_t operator()(const array<T, size>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, p0 = 0;
    while ((p = s.find(sep, p0)) != string::npos) {
        if (p > p0) ret.emplace_back(s.substr(p0, p - p0));
        p0 = p + sep.size();
    }
    if (p0 < s.size()) ret.emplace_back(s.substr(p0));
    return ret;
}

int main()
{
    // Parse input
    vector<vector<pair<char, int>>> wires;
    string line;
    while (getline(cin, line)) {
        auto& wire = wires.emplace_back();
        vector<string> s = split(line, ",");
        for (const string& move : s) {
            assert(move.size() > 1);
            wire.emplace_back(make_pair(move[0], atoi(move.c_str() + 1)));
        }
    }
    assert(wires.size() == 2);

    // Solve part 1 and 2
    // For each position in the grid store which wires have visited
    // it and the sum of the number of steps to get there
    unordered_map<array<int, 2>, array<int, 2>> grid;
    for (int w = 0, b = 1; w < 2; ++w, b <<= 1) {
        const auto& wire = wires[w];
        int row = 0, col = 0, step = 0;
        for (auto [dir, steps] : wire) {
            static const unordered_map<char, array<int, 2>> dirs = {{'R', {0, 1}},
                                                                    {'L', {0, -1}},
                                                                    {'D', {1, 0}},
                                                                    {'U', {-1, 0}}};
            auto iter = dirs.find(dir);
            assert(iter != dirs.end());
            auto [dr, dc] = iter->second;
            for (int s = 0; s < steps; s++) {
                ++step;
                row += dr;
                col += dc;
                auto& g = grid[{row, col}];
                if (!(g[0] & b)) {
                    g[0] |= b;
                    g[1] += step;
                }
            }
        }
    }
    int part1 = INT_MAX;
    int part2 = INT_MAX;
    for (const auto& [pos, state] : grid) {
        if (state[0] == 3) {
            // Both wires have visited this position
            part1 = min(part1, abs(pos[0]) + abs(pos[1]));
            part2 = min(part2, state[1]);
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
