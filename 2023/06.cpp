/*
 * Problem 6, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

int main()
{
    // Parse input data
    vector<array<ll, 2>> races;
    for (size_t i = 0; i < 2; ++i) {
        string line;
        assert(getline(cin, line));
        stringstream ss(line);
        string s;
        ss >> s;
        ll data;
        for (size_t j = 0; ss >> data; ++j) {
            if (races.size() == j) races.emplace_back();
            races[j][i] = data;
        }
    }

    // Part 1
    ll part1 = 1;
    for (const auto& race : races) {
        ll wins = 0;
        for (ll hold = 0; hold <= race[0]; ++hold) {
            ll distance = hold * (race[0] - hold);
            if (distance > race[1]) wins++;
        }
        part1 *= wins;
    }

    // Part 2
    double race_time = 0;
    ll race_mult = 1;
    double record_time = 0;
    ll record_mult = 1;
    for (int i = races.size() - 1; i >= 0; --i) {
        race_time += race_mult * races[i][0];
        race_mult *= pow(10, to_string(races[i][0]).size());
        record_time += record_mult * races[i][1];
        record_mult *= pow(10, to_string(races[i][1]).size());
    }
    double hold_range = sqrt(race_time * race_time / 4 - record_time);
    ll hold_min = ceil((race_time / 2) - hold_range);
    ll hold_max = floor((race_time / 2) + hold_range);
    ll part2 = hold_max - hold_min + 1;

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
