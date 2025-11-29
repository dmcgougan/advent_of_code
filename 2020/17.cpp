/*
 * Problem 17, Advent of Code 2020
 * Danjel McGougan
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_set>
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

// Generic code for arbitrary number of dimensions N
template <size_t N>
static void step(unordered_set<array<int, N>>& grid)
{
    if (grid.empty()) return;

    // Find boundary of where the cubes live
    array<pair<int, int>, N> min_max;
    min_max.fill({INT_MAX, INT_MIN});
    for (const auto& a : grid) {
        for (size_t n = 0; n < N; ++n) {
            auto& [mn, mx] = min_max[n];
            mn = min(mn, a[n]);
            mx = max(mx, a[n]);
        }
    }

    // Recurse over all possible cube positions
    vector<array<int, N>> add, remove;
    array<int, N> pos;
    auto pos_rec = [&](auto& self, size_t i) -> void {
        for (int v = min_max[i].first - 1; v <= min_max[i].second + 1; ++v) {
            pos[i] = v;
            if (i + 1 < N) {
                self(self, i + 1);
                continue;
            }
            // Recurse over all directions and count active neighbors
            int active = 0;
            array<int, N> dir;
            auto dir_rec = [&](auto& self, size_t i) -> void {
                for (int v = -1; v <= 1; ++v) {
                    dir[i] = v;
                    if (i + 1 < N) {
                        self(self, i + 1);
                        continue;
                    }
                    if (all_of(dir.begin(), dir.end(), [](int x) { return x == 0; })) continue;
                    array<int, N> npos;
                    transform(pos.begin(), pos.end(), dir.begin(), npos.begin(), [](int a, int b) {
                        return a + b;
                    });
                    active += grid.count(npos);
                }
            };
            dir_rec(dir_rec, 0);
            // Check how the grid should be modified
            if (grid.count(pos)) {
                if (active != 2 && active != 3) remove.push_back(pos);
            } else {
                if (active == 3) add.push_back(pos);
            }
        }
    };
    pos_rec(pos_rec, 0);

    // Modify the grid
    for (const auto& a : add) grid.insert(a);
    for (const auto& a : remove) grid.erase(a);
}

int main()
{
    // Parse input
    unordered_set<array<int, 3>> grid_p1;
    unordered_set<array<int, 4>> grid_p2;
    string line;
    for (int y = 0; getline(cin, line); ++y) {
        for (int x = 0; x < int(line.size()); ++x) {
            if (line[x] == '#') {
                grid_p1.insert({x, y, 0});
                grid_p2.insert({x, y, 0, 0});
            }
        }
    }

    // Solve part 1
    for (int i = 0; i < 6; ++i) step(grid_p1);
    int part1 = grid_p1.size();

    // Solve part 2
    for (int i = 0; i < 6; ++i) step(grid_p2);
    int part2 = grid_p2.size();

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
