/*
 * Problem 4, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<string> grid;
    for (string line; getline(in, line);) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    int rows = grid.size();
    int cols = grid[0].size();
    assert(rows && cols);

    // Common data
    auto count_removed = [&](bool keep = true) -> int {
        int tot = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '@') continue;
                int count = 0;
                static const int D[][2] =
                    {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
                for (auto [dr, dc] : D) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    if (grid[nr][nc] == '@') ++count;
                }
                if (count < 4) {
                    if (!keep) grid[r][c] = '.';
                    ++tot;
                }
            }
        }
        return tot;
    };

    // Solve part 1
    int part1 = count_removed();

    // Solve part 2
    int part2 = 0;
    {
        int c;
        while ((c = count_removed(false))) part2 += c;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
