/*
 * Problem 17, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>

using namespace std;

static int part2 = 0;

static int height(int xv, int yv, int min_x, int max_x, int min_y, int max_y)
{
    int x = 0;
    int y = 0;
    int max_h = 0;
    do {
        x += xv;
        y += yv;
        max_h = max(max_h, y);
        if (x >= min_x && x <= max_x && y >= min_y && y <= max_y) {
            ++part2;
            return max_h;
        }
        if (xv) xv += xv > 0 ? -1 : 1;
        --yv;
    } while (y >= min_y);
    return INT_MIN;
}

static int max_height(int min_x, int max_x, int min_y, int max_y)
{
    int max_h = INT_MIN;
    for (int yv = -(max_y - min_y + 1) * 10; yv <= (max_y - min_y + 1) * 10; yv++) {
        for (int xv = 1; xv <= max_x; xv++) {
            max_h = max(max_h, height(xv, yv, min_x, max_x, min_y, max_y));
        }
    }
    return max_h;
}

int main()
{
    string line;
    getline(cin, line);
    int min_x, max_x;
    int min_y, max_y;
    assert(
        sscanf(line.c_str(), "target area: x=%d..%d, y=%d..%d", &min_x, &max_x, &min_y, &max_y) ==
        4);

    int part1 = max_height(min_x, max_x, min_y, max_y);
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
