/*
 * Problem 4, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

int main()
{
    // Parse input
    int from, to;
    assert(cin >> from);
    assert(cin.ignore(1));
    assert(cin >> to);
    assert(from <= to && from >= 100000 && to <= 999999);

    // Solve part 1
    int part1 = 0;
    for (int p = from; p <= to; ++p) {
        unsigned pass = p;
        int prev = 10;
        bool good = false;
        while (pass > 0) {
            int d = pass % 10;
            pass /= 10;
            if (d > prev) {
                good = false;
                break;
            }
            if (d == prev) good = true;
            prev = d;
        }
        if (good) ++part1;
    }

    // Solve part 1
    int part2 = 0;
    for (int p = from; p <= to; ++p) {
        unsigned pass = p;
        int prev = 10;
        bool nondec = true;
        bool dbl = false;
        int same = 1;
        while (pass > 0) {
            int d = pass % 10;
            pass /= 10;
            if (d > prev) {
                nondec = false;
                break;
            }
            if (d == prev) {
                ++same;
            } else {
                if (same == 2) dbl = true;
                same = 1;
            }
            prev = d;
        }
        if (same == 2) dbl = true;
        if (nondec && dbl) ++part2;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
