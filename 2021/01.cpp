/*
 * Problem 1, Advent of Code 2021
 * Danjel McGougan
 */

#include <iostream>

using namespace std;

int main()
{
    int part1 = 0;
    int part2 = 0;

    int p = -1;
    int pp = -1;
    int ppp = -1;
    int v;
    while (cin >> v) {
        if (p >= 0 && v > p) part1++;
        if (ppp >= 0 && ppp < v) part2++;
        ppp = pp;
        pp = p;
        p = v;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
