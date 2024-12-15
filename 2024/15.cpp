/*
 * Problem 15, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = int64_t;

static pair<int, int> dir(char m)
{
    switch (m) {
    case '>':
        return {0, 1};
    case 'v':
        return {1, 0};
    case '<':
        return {0, -1};
    case '^':
        return {-1, 0};
    default:
        assert(false);
    }
}

// Move the robot in the grid
static void move_robot(char m, vector<string>& grid, int& r, int& c)
{
    int rows = grid.size();
    int cols = grid[0].size();

    // Find first empty position in the direction the robot is going to move
    auto [dr, dc] = dir(m);
    assert(grid[r][c] == '@');
    int er = r;
    int ec = c;
    do {
        er += dr;
        ec += dc;
        // We should not get outside of the grid due to walls all around
        assert(er >= 0 && er < rows && ec >= 0 && ec < cols);
        if (grid[er][ec] == '#') {
            // Found a wall; robot does not move
            return;
        }
    } while (grid[er][ec] != '.');

    // Move stones and robot
    do {
        grid[er][ec] = grid[er - dr][ec - dc];
        er -= dr;
        ec -= dc;
    } while (grid[er][ec] != '@');

    // Update robot position
    grid[r][c] = '.';
    r += dr;
    c += dc;
    assert(grid[r][c] == '@');
}

// Check if the robot/stone can move one step up/down from the given position
static bool check_move(int dr, int r, int c, const vector<string>& grid)
{
    char t1 = grid[r + dr][c];
    if (grid[r][c] == '@') {
        if (t1 == '.') return true;
        if (t1 == ']') return check_move(dr, r + dr, c - 1, grid);
        if (t1 == '[') return check_move(dr, r + dr, c, grid);
        assert(t1 == '#');
        return false;
    }
    assert(grid[r][c] == '[');
    char t2 = grid[r + dr][c + 1];
    if (t1 == '#' || t2 == '#') return false;
    if (t1 == '[') return check_move(dr, r + dr, c, grid);
    if (t1 == ']') {
        if (!check_move(dr, r + dr, c - 1, grid)) return false;
    }
    if (t2 == '[') return check_move(dr, r + dr, c + 1, grid);
    return true;
}

// Move a stone and all stones it is pushing one step up/down
static void move_stone(int dr, int r, int c, vector<string>& grid)
{
    assert(grid[r][c] == '[' && grid[r][c + 1] == ']');

    // Push stones that are in the way
    if (grid[r + dr][c] == ']') {
        move_stone(dr, r + dr, c - 1, grid);
    } else if (grid[r + dr][c] == '[') {
        move_stone(dr, r + dr, c, grid);
    }
    if (grid[r + dr][c + 1] == '[') {
        move_stone(dr, r + dr, c + 1, grid);
    }

    // Move the stone
    assert(grid[r][c] == '[' && grid[r][c + 1] == ']');
    assert(grid[r + dr][c] == '.' && grid[r + dr][c + 1] == '.');
    grid[r][c] = '.';
    grid[r][c + 1] = '.';
    grid[r + dr][c] = '[';
    grid[r + dr][c + 1] = ']';
}

// Move the robot up/down in the wide grid
static void move_updown(char m, vector<string>& grid, int& r, int c)
{
    assert(grid[r][c] == '@');
    int dr = (m == '^' ? -1 : 1);

    // Check if it is possible to move this direction
    if (!check_move(dr, r, c, grid)) return;

    // Push all stones
    if (grid[r + dr][c] == ']') {
        move_stone(dr, r + dr, c - 1, grid);
    } else if (grid[r + dr][c] == '[') {
        move_stone(dr, r + dr, c, grid);
    }

    // Move the robot
    assert(grid[r][c] == '@');
    assert(grid[r + dr][c] == '.');
    grid[r][c] = '.';
    grid[r + dr][c] = '@';
    r += dr;
}

// Calculate the GPS coordinate sum
static int gps_sum(const vector<string>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();

    int sum = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'O' || grid[r][c] == '[') sum += 100 * r + c;
        }
    }

    return sum;
}

int main()
{
    // Read the input and find the robot position
    vector<string> grid;
    vector<string> wgrid;
    string line;
    int rows = 0;
    int r = 0;
    int c = 0;
    while (getline(cin, line) && !line.empty()) {
        size_t ix = line.find_first_of('@');
        if (ix != string::npos) {
            r = rows;
            c = ix;
        }
        string wline;
        for (char tile : line) {
            switch (tile) {
            case '#':
                wline += "##";
                break;
            case 'O':
                wline += "[]";
                break;
            case '.':
                wline += "..";
                break;
            case '@':
                wline += "@.";
                break;
            default:
                assert(false);
            }
        }
        grid.emplace_back(move(line));
        wgrid.emplace_back(move(wline));
        rows++;
    }
    int wr = r;
    int wc = 2 * c;
    assert(grid[r][c] == '@');
    assert(wgrid[wr][wc] == '@');
    string moves;
    while (getline(cin, line)) {
        moves += line;
    }

    // Move robot around the grid
    for (char m : moves) {
        // Part 1
        move_robot(m, grid, r, c);

        // Part 2
        if (m == '<' || m == '>') {
            // Horizontal movement: same code as for part 1
            move_robot(m, wgrid, wr, wc);
        } else {
            // Vertical movement
            move_updown(m, wgrid, wr, wc);
        }
    }

    cout << "Part 1: " << gps_sum(grid) << endl;
    cout << "Part 2: " << gps_sum(wgrid) << endl;

    return 0;
}
