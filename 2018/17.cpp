/*
 * Problem 17, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
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

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> result;
    size_t start = 0, p;
    while ((p = s.find(sep, start)) != string::npos) {
        if (p > start) result.emplace_back(s.substr(start, p - start));
        start = p + sep.size();
    }
    if (start < s.size()) result.emplace_back(s.substr(start));
    return result;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<string> grid;
    int rows;
    int cols;
    int spring_row;
    int spring_col;
    int min_row;
    {
        vector<array<int, 4>> clay;
        int min_x = INT_MAX, max_x = INT_MIN;
        int min_y = INT_MAX, max_y = INT_MIN;
        for (string line; getline(in, line);) {
            vector<string> s;
            CHECK((s = split(line, ", ")).size() == 2 && s[0].size() > 2 && s[1].size() > 2);
            auto& p = clay.emplace_back();
            p[0] = s[0][0];  // 'x' or 'y'
            p[1] = atoi(s[0].c_str() + 2);
            if (p[0] == 'x') {
                min_x = min(min_x, p[1]);
                max_x = max(max_x, p[1]);
            } else {
                min_y = min(min_y, p[1]);
                max_y = max(max_y, p[1]);
            }
            CHECK((s = split(s[1], "..")).size() == 2 && s[0].size() > 2);
            p[2] = atoi(s[0].c_str() + 2);  // from
            p[3] = atoi(s[1].c_str());      // to
            if (p[0] == 'x') {
                min_y = min(min_y, p[2]);
                max_y = max(max_y, p[3]);
            } else {
                min_x = min(min_x, p[2]);
                max_x = max(max_x, p[3]);
            }
        }
        min_row = min_y;
        // Account for the spring source
        min_x = min(min_x, 500);
        max_x = max(max_x, 500);
        min_y = min(min_y, 0);
        max_y = max(max_y, 0);
        // Expand X directions to let water flow around clay at the edges
        --min_x;
        ++max_x;
        rows = max_y - min_y + 1;
        cols = max_x - min_x + 1;
        spring_row = 0 - min_y;
        spring_col = 500 - min_x;
        min_row -= min_y;
        grid = vector<string>(rows, string(cols, '.'));
        for (auto [xy, rc, from, to] : clay) {
            if (xy == 'x') {
                for (int r = from - min_y; r <= to - min_y; ++r) grid[r][rc - min_x] = '#';
            } else {
                for (int c = from - min_x; c <= to - min_x; ++c) grid[rc - min_y][c] = '#';
            }
        }
    }

    // Emit water and drain it down the grid until it settles
    vector<array<int, 2>> st;
    st.push_back({spring_row + 1, spring_col});
    while (!st.empty()) {
        auto [r, c] = st.back();
        st.pop_back();

        // Fall down until we hit an obstacle
        for (;;) {
            if (grid[r][c] == '.') grid[r][c] = '|';
            if (r + 1 >= rows) break;
            if (grid[r + 1][c] == '#' || grid[r + 1][c] == '~') break;
            ++r;
        }
        if (r + 1 >= rows) continue;

        // Spread / settle
        for (;;) {
            int min_c = c;
            int max_c = c;
            int walls = 0;
            for (int dc = -1; dc <= 1; dc += 2) {
                int cc = c;
                for (;;) {
                    int nc = cc + dc;
                    if (nc < 0 || nc >= cols) break;
                    if (grid[r][nc] == '#') {
                        ++walls;
                        break;
                    }
                    cc = nc;
                    min_c = min(min_c, cc);
                    max_c = max(max_c, cc);
                    if (grid[r][cc] == '.') grid[r][cc] = '|';
                    char tile = grid[r + 1][cc];
                    if (tile != '#' && tile != '~') {
                        if (tile == '.') st.push_back({r, cc});
                        break;
                    }
                }
            }
            if (walls < 2) break;
            for (int c = min_c; c <= max_c; ++c) {
                CHECK(grid[r][c] == '|' || grid[r][c] == '~');
                grid[r][c] = '~';
            }
            if (--r <= spring_row || grid[r][c] == '#') break;
            if (grid[r][c] == '.') grid[r][c] = '|';
        }
    }

    // Solve part 1
    int part1 = 0;
    for (int r = min_row; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '|' || grid[r][c] == '~') ++part1;
        }
    }

    // Solve part 2
    int part2 = 0;
    for (int r = min_row; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '~') ++part2;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
