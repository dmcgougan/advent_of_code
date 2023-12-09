/*
 * Problem 9, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

int main()
{
    // Parse input data
    vector<vector<ll>> histories;
    string line;
    while (getline(cin, line)) {
        vector<ll> history;
        stringstream ss(line);
        ll entry;
        while (ss >> entry) history.push_back(entry);
        histories.emplace_back(move(history));
    }

    ll part1 = 0;
    ll part2 = 0;
    for (vector<ll>& seq : histories) {
        // Reduce the sequence stepwise until there is only zeroes left.
        // For part1 we just add up all the last entries of the sequences.
        // For part2 the result should be a sum where we alternatively add and
        // subtract the first entry of each sequence starting from the last
        // sequence. Implement it by doing it from the first sequence and
        // correct the sign at the end depending on if we had an even or odd
        // number of steps.
        ll result1 = 0;
        ll result2 = 0;
        int steps = 0;
        for (;;) {
            result1 += seq.back();
            result2 = -result2 + seq.front();
            bool all_zeroes = true;
            for (size_t i = 1; i < seq.size(); ++i) {
                if ((seq[i - 1] = seq[i] - seq[i - 1]) != 0) all_zeroes = false;
            }
            if (all_zeroes) break;
            seq.pop_back();
            ++steps;
        }

        part1 += result1;
        part2 += steps % 2 ? -result2 : result2;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
