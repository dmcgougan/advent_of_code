/*
 * Problem 7, Advent of Code 2021
 * Danjel McGougan
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> pos;
    int v;
    int max_val = 0;
    while (cin >> v) {
        pos.push_back(v);
        max_val = max(max_val, v);
        char c;
        cin >> c;
    }

    int part1 = INT_MAX;
    int part2 = INT_MAX;
    for (int p = 0; p <= max_val; p++) {
        int fuel1 = 0;
        int fuel2 = 0;
        for (int v : pos) {
            int dist = abs(v - p);
            fuel1 += dist;
            fuel2 += dist * (dist + 1) / 2;
        }
        part1 = min(part1, fuel1);
        part2 = min(part2, fuel2);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
