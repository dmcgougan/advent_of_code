/*
 * Problem 14, Advent of Code 2024
 * Danjel McGougan
 */

#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = int64_t;

static const int WIDTH = 101;
static const int HEIGHT = 103;

// Simulate one round of robots moving
static void round(vector<tuple<int, int, int, int>>& robots, vector<vector<int>>& grid)
{
    for (auto& [x, y, dx, dy] : robots) {
        grid[y][x]--;
        x = (x + dx + WIDTH) % WIDTH;
        y = (y + dy + HEIGHT) % HEIGHT;
        grid[y][x]++;
    }
}

// Safety factor for part 1
static int safety_factor(const vector<tuple<int, int, int, int>>& robots)
{
    int result[4] = {};
    for (const auto& [x, y, dx, dy] : robots) {
        if (x != WIDTH / 2 && y != HEIGHT / 2) {
            result[2 * (x < WIDTH / 2) + (y < HEIGHT / 2)]++;
        }
    }
    return result[0] * result[1] * result[2] * result[3];
}

// Calculate a very simple metric that is correlated to the grid entropy.
// Here we calculate how many neighbours the robots have in the grid.
// The more neighbours we find the more likely it is some form of image with lower entropy.
static int neighbours(const vector<tuple<int, int, int, int>>& robots,
                      const vector<vector<int>>& grid)
{
    int count = 0;
    for (const auto& [x1, y1, dx1, dy1] : robots) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int x = x1 + dx;
                int y = y1 + dy;
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                    if (grid[y][x]) count++;
                }
            }
        }
    }
    return count;
}

// Display the grid of robots
static void display(const vector<vector<int>>& grid)
{
    for (const vector<int>& row : grid) {
        for (int c : row) {
            cout << (c ? '*' : ' ');
        }
        cout << endl;
    }
}

int main()
{
    string line;
    vector<tuple<int, int, int, int>> robots;
    vector<vector<int>> grid(HEIGHT, vector<int>(WIDTH, 0));
    while (getline(cin, line)) {
        int x, y, dx, dy;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &x, &y, &dx, &dy);
        robots.emplace_back(x, y, dx, dy);
        grid[y][x]++;
    }

    int part1 = 0;
    int part2 = 0;

    // It turns out that the easter egg we search has a neighbour count of over 7000
    // while other grids are in the 200-400 range.
    while (neighbours(robots, grid) < 1000) {
        round(robots, grid);
        part2++;
        if (part2 == 100) {
            part1 = safety_factor(robots);
        }
    }

    // Display the easter egg!
    display(grid);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
