/*
 * Problem 6, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

int main()
{
    // Parse input and solve
    string line;
    int yes[26]{};   // counts how many persons answered yes to the question
    int anyone = 0;  // counts how many questions are answered yes by anyone
    int count = 0;   // how many persons there are in the group
    int part1 = 0;
    int part2 = 0;
    while (cin) {
        if (getline(cin, line) && !line.empty()) {
            // Answers for one person
            ++count;
            for (char c : line) {
                assert(c >= 'a' && c <= 'z');
                anyone += !yes[c - 'a']++;
            }
        } else {
            // Answers for a group have been parsed
            part1 += anyone;
            for (int& y : yes) {
                part2 += (y == count);
                y = 0;
            }
            anyone = 0;
            count = 0;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
