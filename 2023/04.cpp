/*
 * Problem 4, Advent of Code 2023
 * Danjel McGougan
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int part1 = 0;
    int part2 = 0;
    string line;

    deque<int> copies;
    while (getline(cin, line)) {
        stringstream ss(line);
        string s;
        ss >> s;
        ss >> s;
        vector<int> winning;
        vector<int> mine;

        // Parse winning numbers and my numbers
        while (!ss.eof()) {
            ss >> s;
            if (s == "|") break;
            winning.push_back(stoi(s));
        }
        while (!ss.eof()) {
            ss >> s;
            mine.push_back(stoi(s));
        }

        // Check how many matches we have
        int matches = 0;
        for (int n : mine) {
            if (find(winning.begin(), winning.end(), n) != winning.end()) {
                matches++;
            }
        }

        // Calculate score for part 1
        if (matches) part1 += (1 << (matches - 1));

        // Count how many instances we have
        int instances = 1;
        if (copies.size() > 0) {
            instances += copies[0];
            copies.pop_front();
        }
        part2 += instances;

        // Add copies
        for (int i = 0; i < matches; ++i) {
            if (i < (int)copies.size()) {
                copies[i] += instances;
            } else {
                copies.push_back(instances);
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
