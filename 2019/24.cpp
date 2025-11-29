/*
 * Problem 24, Advent of Code 2019
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Hash function for array
template <typename T, size_t N>
struct std::hash<array<T, N>>
{
    size_t operator()(const array<T, N>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = argc > 1 ? (file.open(argv[1]), file) : cin;
    assert(in);

    // Parse input
    vector<string> grid;
    for (string line; getline(in, line);) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    assert(!grid.empty());
    int rows = grid.size();
    int cols = grid[0].size();

    // Common
    static const int D[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    auto rating = [](const vector<string>& g) -> int {
        int result = 0;
        int bit = 1;
        for (size_t r = 0; r < g.size(); ++r) {
            for (size_t c = 0; c < g[r].size(); ++c) {
                if (g[r][c] == '#') result |= bit;
                bit <<= 1;
            }
        }
        return result;
    };

    // Solve part 1
    int part1 = 0;
    {
        vector<string> g = grid;
        unordered_set<int> seen;
        seen.insert(rating(g));
        for (;;) {
            vector<string> old = g;
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    int adj = 0;
                    for (auto [dr, dc] : D) {
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                        if (old[nr][nc] == '#') ++adj;
                    }
                    if (old[r][c] == '#' && adj != 1) {
                        g[r][c] = '.';
                    } else if (old[r][c] == '.' && (adj == 1 || adj == 2)) {
                        g[r][c] = '#';
                    }
                }
            }
            int r = rating(g);
            if (seen.count(r)) {
                part1 = r;
                break;
            }
            seen.insert(r);
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Bugs [ level, row, col ]
        unordered_set<array<int, 3>> bugs;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] == '#') {
                    bugs.insert({0, r, c});
                }
            }
        }
        // Run for 200 minutes
        for (int t = 0; t < 200; ++t) {
            unordered_map<array<int, 3>, int> adj;
            // Determine all tiles adjacent to bugs
            for (auto [lvl, r, c] : bugs) {
                for (auto [dr, dc] : D) {
                    if (r + dr < 0) {
                        ++adj[{lvl - 1, rows / 2 - 1, cols / 2}];
                    } else if (r + dr == rows) {
                        ++adj[{lvl - 1, rows / 2 + 1, cols / 2}];
                    } else if (c + dc < 0) {
                        ++adj[{lvl - 1, rows / 2, cols / 2 - 1}];
                    } else if (c + dc == cols) {
                        ++adj[{lvl - 1, rows / 2, cols / 2 + 1}];
                    } else if (r == rows / 2 && c == cols / 2 - 1 && dc == 1) {
                        for (int rr = 0; rr < rows; ++rr) {
                            ++adj[{lvl + 1, rr, 0}];
                        }
                    } else if (r == rows / 2 && c == cols / 2 + 1 && dc == -1) {
                        for (int rr = 0; rr < rows; ++rr) {
                            ++adj[{lvl + 1, rr, cols - 1}];
                        }
                    } else if (r == rows / 2 - 1 && c == cols / 2 && dr == 1) {
                        for (int cc = 0; cc < cols; ++cc) {
                            ++adj[{lvl + 1, 0, cc}];
                        }
                    } else if (r == rows / 2 + 1 && c == cols / 2 && dr == -1) {
                        for (int cc = 0; cc < cols; ++cc) {
                            ++adj[{lvl + 1, rows - 1, cc}];
                        }
                    } else {
                        ++adj[{lvl, r + dr, c + dc}];
                    }
                }
            }
            // Update bugs
            unordered_set<array<int, 3>> old = bugs;
            for (const auto& [pos, count] : adj) {
                bool bug = old.count(pos) != 0;
                if (bug && count != 1) {
                    bugs.erase(pos);
                } else if (!bug && (count == 1 || count == 2)) {
                    bugs.insert(pos);
                }
            }
            for (const auto& pos : old) {
                auto iter = adj.find(pos);
                if (iter == adj.end()) {
                    // This bug has no neighbors
                    bugs.erase(pos);
                }
            }
        }
        part2 = bugs.size();
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
