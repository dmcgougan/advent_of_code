/*
 * Problem 18, Advent of Code 2024
 * Danjel McGougan
 */

#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

static const int ROWS = 71;
static const int COLS = 71;
static const int FALLEN = 1024;

int main()
{
    string line;
    vector<pair<int, int>> coords;
    vector<string> grid(ROWS, string(COLS, '.'));
    while (getline(cin, line)) {
        stringstream ss(line);
        int col, row;
        char c;
        ss >> col >> c >> row;
        coords.push_back({row, col});
        if (coords.size() <= FALLEN) {
            grid[row][col] = '#';
        }
    }

    int part1 = 0;
    string part2;

    for (size_t index = FALLEN; index < coords.size(); ++index) {
        bool found = false;

        // BFS
        deque<tuple<int, int, int>> queue;
        vector<vector<bool>> visited(ROWS, vector<bool>(COLS));
        queue.push_back({0, 0, 0});
        while (!queue.empty()) {
            auto [steps, r, c] = queue.front();
            queue.pop_front();
            if (visited[r][c]) continue;
            visited[r][c] = true;
            if (r == ROWS - 1 && c == COLS - 1) {
                // Found the end tile
                if (index == FALLEN) part1 = steps;
                found = true;
                break;
            }
            static const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            for (int d = 0; d < 4; d++) {
                int nr = r + dirs[d][0];
                int nc = c + dirs[d][1];
                if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] == '.' &&
                    !visited[nr][nc])
                {
                    queue.push_back({steps + 1, nr, nc});
                }
            }
        }

        if (!found) {
            // No path to end tile
            part2 = to_string(coords[index - 1].second) + ',' + to_string(coords[index - 1].first);
            break;
        }

        // Add a new obstacle
        grid[coords[index].first][coords[index].second] = '#';
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
