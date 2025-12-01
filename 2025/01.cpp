/*
 * Problem 1, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<pair<char, int>> list;
    for (string line; getline(in, line);) {
        list.push_back({line[0], atoi(line.c_str() + 1)});
    }

    // Solve part 1
    int part1 = 0;
    {
        int dial = 50;
        for (auto [dir, rot] : list) {
            dial = (dial + (dir == 'L' ? -rot : rot)) % 100;
            if (dial == 0) ++part1;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        int dial = 50;
        for (auto [dir, rot] : list) {
            for (int i = 0; i < rot; i++) {
                dial = (dial + (dir == 'L' ? -1 : 1)) % 100;
                if (dial == 0) ++part2;
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
