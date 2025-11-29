/*
 * Problem 18, Advent of Code 2019
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
    const int num_keys = 26;

    // Compute an adjacency list for the given position
    // [ destination, distance, doors ]
    auto bfs_from = [&](int from_row, int from_col) -> vector<array<int, 3>> {
        vector<array<int, 3>> adj;
        queue<array<int, 4>> queue;
        vector<vector<bool>> visited(rows, vector<bool>(cols));
        visited[from_row][from_col] = true;
        queue.push({from_row, from_col, 0, 0});
        while (!queue.empty()) {
            auto [row, col, doors, dist] = queue.front();
            queue.pop();
            char to = grid[row][col];
            if (to >= 'a' && to <= 'z') {
                adj.push_back({to - 'a', dist, doors});
            }
            if (to >= 'A' && to <= 'Z') doors |= 1 << (to - 'A');
            static const array<int, 2> dirs[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            for (int dir = 0; dir < 4; ++dir) {
                int nr = row + dirs[dir][0];
                int nc = col + dirs[dir][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] == '#') continue;
                if (visited[nr][nc]) continue;
                visited[nr][nc] = true;
                queue.push({nr, nc, doors, dist + 1});
            }
        }
        return adj;
    };

    // Solve part 1
    int part1 = 0;
    {
        // Compute an adjacency list for every key (+ start pos) in the grid
        // Keys are numbered [0, num_keys - 1], start position is num_keys
        vector<vector<array<int, 3>>> adj(num_keys + 1);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char tile = grid[r][c];
                if ((tile >= 'a' && tile <= 'z') || tile == '@') {
                    tile = (tile == '@' ? num_keys : tile - 'a');
                    adj[tile] = bfs_from(r, c);
                }
            }
        }

        // Dijkstra
        // Minimum priority queue with [ dist, position, keys ]
        priority_queue<array<int, 3>, vector<array<int, 3>>, greater<array<int, 3>>> queue;
        unordered_map<array<int, 2>, int> min_dist;
        queue.push({0, num_keys, 0});
        while (!queue.empty()) {
            auto [dist, from, keys] = queue.top();
            queue.pop();
            if (keys == (1 << num_keys) - 1) {
                // Found all keys
                part1 = dist;
                break;
            }
            auto iter = min_dist.find({from, keys});
            if (iter != min_dist.end() && dist > iter->second) continue;
            for (const auto& [to, d, doors] : adj[from]) {
                if ((keys & (1 << to)) || (doors & ~keys)) continue;
                int new_keys = keys | (1 << to);
                int new_dist = dist + d;
                auto iter = min_dist.find({to, new_keys});
                if (iter != min_dist.end()) {
                    if (new_dist >= iter->second) continue;
                    iter->second = new_dist;
                } else {
                    min_dist[{to, new_keys}] = new_dist;
                }
                queue.push({new_dist, to, new_keys});
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Update the grid
        static const char grid_update[3][4] = {"@#@", "###", "@#@"};
        assert(grid[rows / 2][cols / 2] == '@');
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                grid[rows / 2 - 1 + r][cols / 2 - 1 + c] = grid_update[r][c];
            }
        }

        // Compute an adjacency list for every key (+ start pos) for each robot
        // Keys are numbered [0, num_keys - 1], start position is num_keys
        vector<vector<vector<array<int, 3>>>> adj_all;
        for (int q = 0; q < 4; ++q) {
            auto& adj = adj_all.emplace_back(num_keys + 1, vector<array<int, 3>>());
            for (int r = (q & 1 ? rows / 2 : 0); r < (q & 1 ? rows : rows / 2); ++r) {
                for (int c = (q & 2 ? cols / 2 : 0); c < (q & 2 ? cols : cols / 2); ++c) {
                    char tile = grid[r][c];
                    if ((tile >= 'a' && tile <= 'z') || tile == '@') {
                        tile = (tile == '@' ? num_keys : tile - 'a');
                        adj[tile] = bfs_from(r, c);
                    }
                }
            }
        }

        // Dijkstra
        // Minimum priority queue with [ dist, robots_pos, keys ]
        // This could be improved by orders of magnitude with A* and some decent heuristics
        // but since this runs in less than 0.5 seconds on my computer I did not bother
        priority_queue<array<int, 3>, vector<array<int, 3>>, greater<array<int, 3>>> queue;
        unordered_map<array<int, 2>, int> min_dist;
        auto visit = [&](int dist, int robots_pos, int keys) -> void {
            min_dist[{robots_pos, keys}] = dist;
        };
        auto visited = [&](int dist, int robots_pos, int keys) -> bool {
            auto iter = min_dist.find({robots_pos, keys});
            return iter != min_dist.end() && iter->second <= dist;
        };
        queue.push({0, num_keys << 24 | num_keys << 16 | num_keys << 8 | num_keys, 0});
        while (!queue.empty()) {
            auto [dist, robots_pos, keys] = queue.top();
            queue.pop();
            if (keys == (1 << num_keys) - 1) {
                // Found all keys
                part2 = dist;
                break;
            }
            if (visited(dist - 1, robots_pos, keys)) continue;
            for (int q = 0; q < 4; ++q) {
                for (const auto& [to, d, doors] : adj_all[q][(robots_pos >> (8 * q)) & 0xff]) {
                    if ((keys & (1 << to)) || (doors & ~keys)) continue;
                    int new_robots_pos = (robots_pos & ~(0xff << (8 * q))) | (to << (8 * q));
                    int new_keys = keys | (1 << to);
                    int new_dist = dist + d;
                    if (visited(new_dist, new_robots_pos, new_keys)) continue;
                    visit(new_dist, new_robots_pos, new_keys);
                    queue.push({dist + d, new_robots_pos, new_keys});
                }
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
