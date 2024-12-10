/*
 * Problem 10, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <set>
#include <vector>

using namespace std;

static const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

static vector<string> grid;
static int rows, cols;
static set<pair<int, int>> visited;

static int part1;
static int part2;

static void dfs(int row, int col)
{
    char p = grid[row][col];
    if (p == '9') {
        if (!visited.count(make_pair(row, col))) {
            part1++;
            visited.emplace(row, col);
        }
        part2++;
    }
    for (int i = 0; i < 4; i++) {
        int r = row + dirs[i][0];
        int c = col + dirs[i][1];
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == p + 1) {
            dfs(r, c);
        }
    }
}

int main()
{
    string line;
    while (getline(cin, line)) {
        grid.emplace_back(move(line));
    }
    rows = grid.size();
    cols = grid[0].size();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == '0') {
                visited.clear();
                dfs(r, c);
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
