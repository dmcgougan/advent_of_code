/*
 * Problem 23, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<string> grid;
static int rows;
static int cols;

//
// Brute force search. No algorithmic optimizations.
// Runs in about 45 seconds on my computer.
//
// Very likely there is some nice optimization you can do to this problem given
// the premises of AoC. I will revisit this code later when I have time to find
// a better solution.
//
// A template parameter is used to give the compiler the freedom to remove
// the extra checks from the compiled code for part 2, making it faster.
//
static vector<string> visited;
static int depth = 0;
static int max_depth = 0;
template <bool ice>
static void solve(int r, int c)
{
    if (r == rows - 1) {
        max_depth = max(max_depth, depth);
        return;
    }
    visited[r][c] = 1;
    ++depth;
    if (r + 1 < rows && !visited[r + 1][c]) {
        char ch = grid[r + 1][c];
        if (ch != '#') {
            solve<ice>(r + 1, c);
        }
    }
    if (!visited[r][c + 1]) {
        char ch = grid[r][c + 1];
        if (ch != '#') {
            solve<ice>(r, c + 1);
        }
    }
    if (r - 1 >= 0 && !visited[r - 1][c]) {
        char ch = grid[r - 1][c];
        if (ch != '#' && (!ice || ch != 'v')) {
            solve<ice>(r - 1, c);
        }
    }
    if (!visited[r][c - 1]) {
        char ch = grid[r][c - 1];
        if (ch != '#' && (!ice || ch != '>')) {
            solve<ice>(r, c - 1);
        }
    }
    visited[r][c] = 0;
    --depth;
}

int main()
{
    // Parse input data
    string line;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    rows = grid.size();
    cols = grid[0].size();
    visited = vector(rows, string(cols, '\0'));

    // Part 1
    solve<true>(0, 1);
    int part1 = max_depth;

    // Part 2
    max_depth = 0;
    solve<false>(0, 1);
    int part2 = max_depth;

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
