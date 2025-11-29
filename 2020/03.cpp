/*
 * Problem 3, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static int count(const vector<string>& grid, int right, int down)
{
    int rows = grid.size();
    int cols = grid[0].size();
    int result = 0;
    int r = 0, c = 0;
    while (r < rows) {
        if (grid[r][c % cols] == '#') ++result;
        r += down;
        c += right;
    }
    return result;
}

int main()
{
    // Parse input
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.push_back(move(line));
    }
    assert(!grid.empty());

    // Solve
    int part1 = count(grid, 3, 1);
    int64_t part2 = 1;
    static const pair<int, int> slopes[]{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
    for (auto [right, down] : slopes) {
        part2 *= count(grid, right, down);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
