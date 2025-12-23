/*
 * Problem 5, Advent of Code 2018
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    string polymer;
    assert(getline(in, polymer));

    // Common
    auto measure = [](const string& polymer, char remove = 0) -> int {
        string s;
        for (char c : polymer) {
            if (remove && tolower(c) == remove) continue;
            s.push_back(c);
            while (s.size() > 1) {
                char c0 = s[s.size() - 2];
                char c1 = s[s.size() - 1];
                if (tolower(c0) == tolower(c1) && islower(c0) != islower(c1)) {
                    s.erase(s.size() - 2);
                } else {
                    break;
                }
            }
        }
        return s.size();
    };

    // Solve part 1
    int part1 = 0;
    {
        part1 = measure(polymer);
    }

    // Solve part 2
    int part2 = 0;
    {
        int min_size = INT_MAX;
        for (char c = 'a'; c <= 'z'; ++c) {
            min_size = min(min_size, measure(polymer, c));
        }
        part2 = min_size;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
