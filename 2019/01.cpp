/*
 * Problem 1, Advent of Code 2019
 * Danjel McGougan
 */

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<int> list;
    for (int m; cin >> m; ) {
        list.push_back(m);
    }

    // Solve part 1
    int part1 = 0;
    for (int mass : list) {
        part1 += mass / 3 - 2;
    }

    // Solve part 2
    int part2 = 0;
    for (int mass : list) {
        int fuel = 0;
        do {
            mass = mass / 3 - 2;
            fuel += mass;
        } while (mass >= 9);
        part2 += fuel;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
