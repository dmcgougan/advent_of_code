/*
 * Problem 4, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <string>

using namespace std;
using ll = int64_t;

int main()
{
    string line;
    ll p1_sum = 0;
    ll p2_sum = 0;

    while (getline(cin, line)) {
        int f1, t1, f2, t2;
        assert(sscanf(line.c_str(), "%u-%u,%u-%u", &f1, &t1, &f2, &t2) == 4);
        assert(f1 <= t1);
        assert(f2 <= t2);
        if ((f1 <= f2 && t1 >= t2) || (f2 <= f1 && t2 >= t1)) {
            // One range is fully contained in the other
            p1_sum++;
        }
        if (t1 >= f2 && f1 <= t2) {
            // The ranges overlap
            p2_sum++;
        }
    }

    cout << "Part 1: " << p1_sum << endl;
    cout << "Part 2: " << p2_sum << endl;

    return 0;
}
