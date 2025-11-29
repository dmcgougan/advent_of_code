/*
 * Problem 25, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

int main()
{
    // Parse input
    int pub1, pub2;
    assert(cin >> pub1);
    assert(cin >> pub2);

    // Solve part 1
    int v = 1;
    int loops;
    for (loops = 0; v != pub1; ++loops) v = (v * 7) % 20201227;
    v = 1;
    for (int i = 0; i < loops; ++i) v = (int64_t(v) * pub2) % 20201227;
    int part1 = v;

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
