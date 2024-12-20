/*
 * Problem 20, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Count number of cheats that exist for the given number of saved steps and cheat time
static int num_cheats(const vector<string>& grid,
                      int steps_saved,
                      int cheat_time,
                      int sr,
                      int sc,
                      int er,
                      int ec)
{
    int rows = grid.size();
    int cols = grid[0].size();

    // Calculate distances from start of maze when not cheating
    vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
    deque<tuple<int, int, int>> queue;
    queue.push_back({0, sr, sc});
    while (!queue.empty()) {
        auto [d, r, c] = queue.front();
        queue.pop_front();
        if (dist[r][c] <= d) continue;
        dist[r][c] = d;
        static const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dirs[dir][0];
            int nc = c + dirs[dir][1];
            if (grid[nr][nc] == '#' || dist[nr][nc] <= d + 1) continue;
            queue.push_back({d + 1, nr, nc});
        }
    }

    // Check all valid combinations of cheat starting point and cheat ending point
    int count = 0;
    for (int cheat_sr = 1; cheat_sr < rows - 1; ++cheat_sr) {
        for (int cheat_sc = 1; cheat_sc < cols - 1; ++cheat_sc) {
            if (dist[cheat_sr][cheat_sc] == INT_MAX) continue;

            for (int cheat_er = 1; cheat_er < rows - 1; ++cheat_er) {
                for (int cheat_ec = 1; cheat_ec < cols - 1; ++cheat_ec) {
                    if (dist[cheat_er][cheat_ec] == INT_MAX) continue;

                    // Distance to travel between cheat starting point and cheat ending point
                    int cheat_dist = abs(cheat_sr - cheat_er) + abs(cheat_sc - cheat_ec);
                    if (cheat_dist > cheat_time) continue;

                    // Distance from start of maze to cheat starting point
                    int steps = dist[cheat_sr][cheat_sc];

                    // Distance while cheating
                    steps += cheat_dist;

                    // Distance from cheat ending point to end of maze
                    steps += dist[er][ec] - dist[cheat_er][cheat_ec];

                    // Count the cheat if the number of steps is small enough
                    if (steps <= dist[er][ec] - steps_saved) count++;
                }
            }
        }
    }

    return count;
}

int main()
{
    string line;
    vector<string> grid;
    int sr = 0, sc = 0;
    int er = 0, ec = 0;
    int rows = 0;
    while (getline(cin, line)) {
        size_t i = line.find_first_of('S');
        if (i != string::npos) {
            sr = rows;
            sc = i;
        }
        i = line.find_first_of('E');
        if (i != string::npos) {
            er = rows;
            ec = i;
        }
        grid.emplace_back(move(line));
        rows++;
    }
    assert(grid[sr][sc] == 'S');
    assert(grid[er][ec] == 'E');

    cout << "Part 1: " << num_cheats(grid, 100, 2, sr, sc, er, ec) << endl;
    cout << "Part 2: " << num_cheats(grid, 100, 20, sr, sc, er, ec) << endl;

    return 0;
}
