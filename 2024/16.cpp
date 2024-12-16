/*
 * Problem 16, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

int main()
{
    vector<string> grid;
    string line;
    int rows = 0;
    // Start position
    int sr = 0;
    int sc = 0;
    // End position
    int er = 0;
    int ec = 0;
    while (getline(cin, line)) {
        size_t ix = line.find_first_of('S');
        if (ix != string::npos) {
            sc = ix;
            sr = rows;
        }
        ix = line.find_first_of('E');
        if (ix != string::npos) {
            ec = ix;
            er = rows;
        }
        grid.emplace_back(move(line));
        rows++;
    }
    assert(grid[sr][sc] == 'S');
    assert(grid[er][ec] == 'E');

    // Minimum score
    int min_score = INT_MAX;

    // Minimum priority queue with (score, row, col, dir, prev_row, prev_col, prev_dir)
    priority_queue<tuple<int, int, int, int, int, int, int>,
                   vector<tuple<int, int, int, int, int, int, int>>,
                   greater<tuple<int, int, int, int, int, int, int>>>
        queue;

    // Map (row, col, dir) to lowest score
    map<tuple<int, int, int>, int> node_score;

    // Map (row, col, dir) to set of (prev_row, prev_col, prev_dir)
    map<tuple<int, int, int>, set<tuple<int, int, int>>> parents;

    // Starting node
    queue.push({0, sr, sc, 0, -1, -1, -1});

    // Dijkstra
    while (!queue.empty()) {
        auto [score, r, c, d, pr, pc, pd] = queue.top();
        queue.pop();
        auto iter = node_score.find({r, c, d});
        if (iter != node_score.end() && score > iter->second) {
            // Already found cheaper route to this node
            continue;
        }
        node_score[{r, c, d}] = score;

        // Check if we are at the goal
        if (grid[r][c] == 'E') {
            if (score > min_score) {
                // We have already found all best paths; exit the loop
                break;
            }
            min_score = score;
        }

        // Remember the path we took
        if (pr >= 0) {
            parents[{r, c, d}].insert({pr, pc, pd});
        }

        // Insert new nodes into the priority queue
        static const int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        int check_dir[3] = {d, (d + 3) % 4, (d + 1) % 4};
        for (int i = 0; i < 3; i++) {
            int nd = check_dir[i];
            int nr = r + dir[nd][0];
            int nc = c + dir[nd][1];
            if (grid[nr][nc] == '#') continue;
            if (nd == d) {
                // Walk
                queue.push({score + 1, nr, nc, d, r, c, d});
            } else {
                // Turn
                queue.push({score + 1000, r, c, nd, r, c, d});
            }
        }
    }

    // Backtrack to get all tiles that are part of a lowest score path
    set<pair<int, int>> best_tiles;
    {
        deque<tuple<int, int, int>> queue;
        set<tuple<int, int, int>> visited;
        queue.push_back({er, ec, 0});
        queue.push_back({er, ec, 1});
        queue.push_back({er, ec, 2});
        queue.push_back({er, ec, 3});
        while (!queue.empty()) {
            auto [r, c, d] = queue.front();
            queue.pop_front();
            if (visited.count({r, c, d})) continue;
            visited.insert({r, c, d});
            best_tiles.insert({r, c});
            auto iter = parents.find({r, c, d});
            if (iter == parents.end()) continue;
            for (const auto& [pr, pc, pd] : iter->second) {
                queue.push_back({pr, pc, pd});
            }
        }
    }

    cout << "Part 1: " << min_score << endl;
    cout << "Part 2: " << best_tiles.size() << endl;

    return 0;
}
