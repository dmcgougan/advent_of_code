/*
 * Problem 15, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

static int calc_cost(const vector<string>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();

    // Minimum priority queue with [ cost, row, col ]
    priority_queue<tuple<int, int, int>,
                   vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>>
        queue;

    // Starting node
    queue.push({0, 0, 0});
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    visited[0][0] = true;

    // Dijkstra
    int result = -1;
    static const pair<int, int> dirs[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    while (!queue.empty()) {
        auto [cost, r, c] = queue.top();
        queue.pop();
        if (r == rows - 1 && c == cols - 1) {
            // Found the goal
            result = cost;
            break;
        }
        for (auto [dr, dc] : dirs) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (!visited[nr][nc]) {
                queue.push({cost + grid[nr][nc] - '0', nr, nc});
                visited[nr][nc] = true;
            }
        }
    }

    return result;
}

int main()
{
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        assert(grid.empty() || grid[0].size() == line.size());
        grid.push_back(move(line));
    }
    int rows = grid.size();
    int cols = grid[0].size();

    // Part 1
    int part1 = calc_cost(grid);

    // Expand the grid
    for (int r = 0; r < rows; ++r) {
        for (int i = 0; i < 4; i++) {
            for (int c = i * cols; c < (i + 1) * cols; ++c) {
                grid[r] += (grid[r][c] < '9' ? grid[r][c] + 1 : '1');
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int r = i * rows; r < (i + 1) * rows; ++r) {
            grid.push_back(grid[r]);
            for (char& c : grid.back()) {
                c = (c < '9' ? c + 1 : '1');
            }
        }
    }

    // Part 2
    int part2 = calc_cost(grid);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
