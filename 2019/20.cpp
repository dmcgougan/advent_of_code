/*
 * Problem 20, Advent of Code 2019
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <iostream>
#include <queue>
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

int main()
{
    // Parse input
    vector<string> grid;
    for (string line; getline(cin, line);) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(move(line));
    }
    assert(!grid.empty());
    int rows = grid.size();
    int cols = grid[0].size();

    // Find the portals + start/end positions
    unordered_map<array<int, 2>, array<int, 2>> portals;
    unordered_map<array<int, 2>, int> portal_type;  // 0 - inner, 1 - outer
    int start_row, start_col;
    int end_row, end_col;
    {
        auto is_ptile = [](char c) -> bool { return c >= 'A' && c <= 'Z'; };
        unordered_map<int, array<int, 2>> endpoint;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char t0 = grid[r][c];
                if (!is_ptile(t0)) continue;
                char t1;
                int pr, pc, ptype;
                if (c + 1 < cols && is_ptile((t1 = grid[r][c + 1]))) {
                    bool left = (c - 1 >= 0 && grid[r][c - 1] == '.');
                    assert(left || (c + 2 < cols && grid[r][c + 2] == '.'));
                    pr = r;
                    pc = left ? c - 1 : c + 2;
                    ptype = left ? c + 2 == cols : c == 0;
                } else if (r + 1 < rows && is_ptile((t1 = grid[r + 1][c]))) {
                    bool above = (r - 1 >= 0 && grid[r - 1][c] == '.');
                    assert(above || (r + 2 < rows && grid[r + 2][c] == '.'));
                    pr = above ? r - 1 : r + 2;
                    pc = c;
                    ptype = above ? r + 2 == rows : r == 0;
                } else {
                    continue;
                }
                assert(grid[pr][pc] == '.');
                int p = t0 << 8 | t1;
                auto iter = endpoint.find(p);
                if (iter == endpoint.end()) {
                    endpoint[p] = {pr, pc};
                } else {
                    portals[{pr, pc}] = iter->second;
                    portals[iter->second] = {pr, pc};
                    endpoint.erase(iter);
                }
                if (p != ('A' << 8 | 'A') && p != ('Z' << 8 | 'Z')) {
                    portal_type[{pr, pc}] = ptype;
                }
            }
        }
        // Extract start and end points
        assert(endpoint.size() == 2);
        auto iter = endpoint.find('A' << 8 | 'A');
        assert(iter != endpoint.end());
        start_row = iter->second[0];
        start_col = iter->second[1];
        iter = endpoint.find('Z' << 8 | 'Z');
        assert(iter != endpoint.end());
        end_row = iter->second[0];
        end_col = iter->second[1];
    }

    // Solve part 1
    int part1 = 0;
    {
        // BFS with queue [ row, col, dist ]
        queue<array<int, 3>> queue;
        vector<vector<bool>> visited(rows, vector<bool>(cols));
        visited[start_row][start_col] = true;
        queue.push({start_row, start_col, 0});
        while (!queue.empty()) {
            auto [row, col, dist] = queue.front();
            queue.pop();
            if (row == end_row && col == end_col) {
                // Found the end tile
                part1 = dist;
                break;
            }
            static const array<int, 2> dirs[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            for (int dir = 0; dir < 4; ++dir) {
                int nr = row + dirs[dir][0];
                int nc = col + dirs[dir][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] != '.') continue;
                if (visited[nr][nc]) continue;
                visited[nr][nc] = true;
                auto iter = portals.find({nr, nc});
                if (iter == portals.end()) {
                    queue.push({nr, nc, dist + 1});
                } else {
                    // Go through the portal; this takes an extra step
                    queue.push({iter->second[0], iter->second[1], dist + 2});
                }
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // BFS with queue [ row, col, level, dist ]
        queue<array<int, 4>> queue;
        vector<vector<unordered_set<int>>> visited(rows, vector<unordered_set<int>>(cols));
        visited[start_row][start_col].insert(0);
        queue.push({start_row, start_col, 0, 0});
        while (!queue.empty()) {
            auto [row, col, level, dist] = queue.front();
            queue.pop();
            if (row == end_row && col == end_col && level == 0) {
                // Found the end tile
                part2 = dist;
                break;
            }
            static const array<int, 2> dirs[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            for (int dir = 0; dir < 4; ++dir) {
                int nr = row + dirs[dir][0];
                int nc = col + dirs[dir][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] != '.') continue;
                if (visited[nr][nc].count(level)) continue;
                visited[nr][nc].insert(level);
                auto iter = portal_type.find({nr, nc});
                if (iter == portal_type.end() || (level == 0 && iter->second)) {
                    queue.push({nr, nc, level, dist + 1});
                } else {
                    // Go through the portal
                    auto [pr, pc] = portals[{nr, nc}];
                    queue.push({pr, pc, level + (iter->second ? -1 : 1), dist + 2});
                }
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
