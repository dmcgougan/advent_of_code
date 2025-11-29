/*
 * Problem 5, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using ll = int64_t;

int main()
{
    int part1 = 0;
    int part2 = 0;

    map<pair<int, int>, int> map1;
    map<pair<int, int>, int> map2;

    string line;
    while (getline(cin, line)) {
        int x1, y1, x2, y2;
        sscanf(line.c_str(), "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
        int dx = (x1 < x2) - (x2 < x1);  // -1, 0, 1
        int dy = (y1 < y2) - (y2 < y1);  // -1, 0, 1
        int x = x1;
        int y = y1;
        for (;;) {
            if ((dx == 0 || dy == 0)) {
                if (++map1[{x, y}] == 2) part1++;
            }
            if (++map2[{x, y}] == 2) part2++;
            if (x == x2 && y == y2) break;
            x += dx;
            y += dy;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
