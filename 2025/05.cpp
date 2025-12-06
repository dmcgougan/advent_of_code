/*
 * Problem 5, Advent of Code 2025
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<pair<ll, ll>> fresh;
    vector<ll> ingr;
    for (string line; getline(in, line) && !line.empty();) {
        ll from, to;
        assert(sscanf(line.c_str(), "%lld-%lld", &from, &to) == 2);
        fresh.push_back({from, to});
    }
    for (string line; getline(in, line);) {
        ingr.push_back(atoll(line.c_str()));
    }
    assert(!fresh.empty() && !ingr.empty());

    // Solve part 1
    int part1 = 0;
    {
        for (ll i : ingr) {
            for (auto [from, to] : fresh) {
                if (i >= from && i <= to) {
                    ++part1;
                    break;
                }
            }
        }
    }

    // Solve part 2
    ll part2 = 0;
    {
        // Sort intervals in ascending order
        std::sort(fresh.begin(), fresh.end());

        // Highest number we have accounted for and seen so far
        ll cur = -1;
        for (auto [from, to] : fresh) {
            // from is in sorted order
            if (from > cur) {
                // cur is lower than the interval
                //   *  |------|
                // Add the whole interval
                part2 += to - from + 1;
                cur = to;
            } else if (to > cur) {
                // cur is inside the interval
                //   |---*--|
                // Add only the part of the interval that is new
                part2 += to - cur;
                cur = to;
            } else {
                //  cur is higher than the interval
                //    |------|  *
                // Already accounted for
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
