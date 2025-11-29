/*
 * Problem 5, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

int main()
{
    // Parse input and solve part 1
    string line;
    bool ids[1 << 10]{};
    int part1 = 0;
    while (getline(cin, line)) {
        assert(line.size() == 10);
        int id = 0;
        for (int i = 0, b = 1 << 9; i < 10; ++i, b >>= 1) {
            if (line[i] == 'B' || line[i] == 'R') id |= b;
        }
        part1 = max(part1, id);
        ids[id] = true;
    }

    // Solve part 2
    int part2 = 0;
    for (int id = 1; id < (1 << 10) - 1; ++id) {
        if (!ids[id] && ids[id - 1] && ids[id + 1]) {
            part2 = id;
            break;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
