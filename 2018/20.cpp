/*
 * Problem 20, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, unsigned line = source_location::current().line())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(line);
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    string regex;
    CHECK(getline(in, regex));
    CHECK(regex.size() >= 2 && regex.front() == '^' && regex.back() == '$');

    // Common
    vector<vector<int>> dist;
    int rows;
    int cols;
    {
        static const unordered_map<char, array<int, 2>> D = {{'N', {-1, 0}},
                                                             {'E', {0, 1}},
                                                             {'S', {1, 0}},
                                                             {'W', {0, -1}}};

        int rpos = 0;
        int min_row = INT_MAX, max_row = INT_MIN;
        int min_col = INT_MAX, max_col = INT_MIN;
        auto traverse = [&](auto self, int row, int col, vector<string>* grid = nullptr) -> void {
            int srow = row;
            int scol = col;
            for (;;) {
                if (!grid) {
                    min_row = min(min_row, row);
                    max_row = max(max_row, row);
                    min_col = min(min_col, col);
                    max_col = max(max_col, col);
                }
                char c = regex[++rpos];
                if (c == '$') {
                    break;
                }
                if (c == ')') {
                    break;
                }
                if (c == '|') {
                    row = srow;
                    col = scol;
                    continue;
                }
                if (c == '(') {
                    self(self, row, col, grid);
                    continue;
                }
                auto it = D.find(c);
                CHECK(it != D.end());
                int nr = row + 2 * it->second[0];
                int nc = col + 2 * it->second[1];
                if (grid) {
                    // Fill in the door and the room
                    (*grid)[row + it->second[0]][col + it->second[1]] =
                        (c == 'N' || c == 'S' ? '-' : '|');
                    (*grid)[nr][nc] = '.';
                }
                row = nr;
                col = nc;
            }
        };

        // Traverse once to get the grid size
        traverse(traverse, 0, 0);

        // Allocate the grid
        rows = max_row - min_row + 3;
        cols = max_col - min_col + 3;
        vector<string> grid = vector(rows, string(cols, '#'));

        // Traverse again to fill in the grid
        rpos = 0;
        traverse(traverse, 1 - min_row, 1 - min_col, &grid);

        // BFS to find shortest path to all positions
        dist = vector(rows, vector<int>(cols, -1));
        queue<array<int, 2>> q;
        q.push({1 - min_row, 1 - min_col});
        dist[1 - min_row][1 - min_col] = 0;
        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();
            for (const auto& [_, dir] : D) {
                auto [dr, dc] = dir;
                if (grid[row + dr][col + dc] == '#') continue;
                int nr = row + 2 * dr;
                int nc = col + 2 * dc;
                if (dist[nr][nc] >= 0) continue;
                q.push({nr, nc});
                dist[nr][nc] = dist[row][col] + 1;
            }
        }
    }

    // Solve part 1
    int part1 = 0;
    {
        for (int r = 1; r < rows; r += 2) {
            for (int c = 1; c < cols; c += 2) {
                part1 = max(part1, dist[r][c]);
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        for (int r = 1; r < rows; r += 2) {
            for (int c = 1; c < cols; c += 2) {
                if (dist[r][c] >= 1000) ++part2;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
