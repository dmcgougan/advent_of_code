/*
 * Problem 16, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <map>
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
    int r = 0;
    int c = 0;
    while (getline(cin, line)) {
        size_t ix = line.find_first_of('S');
        if (ix != string::npos) {
            c = ix;
            r = rows;
        }
        grid.emplace_back(move(line));
        rows++;
    }
    int sr = r;
    int sc = c;
    assert(grid[sr][sc] == 'S');

    int part1 = INT_MAX;
    set<pair<int, int>> best_path;
    map<tuple<int, int, int>, int> min_score;
    deque<vector<tuple<int, int, int, int>>> queue;

    // Code runs in about 4 seconds on my computer
    // There is probably some clever way to not have to store whole paths in the queue

    // Run BFS twice; one to get the best score and the second time to store path nodes
    for (int i = 0; i < 2; i++) {
        queue.push_back(vector<tuple<int, int, int, int>>());
        queue.back().push_back({sr, sc, 0, 0});
        while (!queue.empty()) {
            auto path = move(queue.front());
            queue.pop_front();

            auto [r, c, d, score] = path.back();
            auto iter = min_score.find({r, c, d});
            if (iter != min_score.end() && iter->second < score) {
                continue;
            }
            min_score[{r, c, d}] = score;

            // Check if we are at the goal
            if (grid[r][c] == 'E') {
                if (score <= part1) {
                    part1 = score;
                    if (i == 1) {
                        // Store nodes used for this path
                        for (const auto& [r, c, d, score] : path) {
                            best_path.insert({r, c});
                        }
                    }
                }
                continue;
            }

            // Start with a turn 90 degrees counterclockwise
            static const int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            int nd = (d + 3) % 4;
            for (int j = 0; j < 3; j++) {
                // Try available directions
                int nr = r + dir[nd][0];
                int nc = c + dir[nd][1];
                if (grid[nr][nc] == '.' || grid[nr][nc] == 'E') {
                    if (j == 1) {
                        // Same direction
                        path.push_back({nr, nc, nd, score + 1});
                        queue.push_back(path);
                        path.pop_back();
                    } else {
                        // New direction
                        path.push_back({r, c, nd, score + 1000});
                        queue.push_back(path);
                        path.pop_back();
                    }
                }
                // Next direction
                nd = (nd + 1) % 4;
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << best_path.size() << endl;

    return 0;
}
