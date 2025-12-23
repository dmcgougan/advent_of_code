/*
 * Problem 7, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <climits>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 2>> order;
    for (string line; getline(in, line);) {
        char before, after;
        assert(sscanf(line.c_str(),
                      "Step %c must be finished before step %c can begin.",
                      &before,
                      &after) == 2);
        order.push_back({before, after});
    }
    assert(!order.empty());

    // Common data
    unordered_map<char, unordered_set<char>> steps;
    for (auto [before, after] : order) {
        steps[after].insert(before);
        steps[before];
    }

    // Solve part 1
    string part1;
    {
        unordered_map<char, unordered_set<char>> s = steps;
        while (!s.empty()) {
            char min_step = CHAR_MAX;
            for (const auto& [step, before] : s) {
                if (before.empty()) {
                    // This step has nothing before it
                    min_step = min(min_step, step);
                }
            }
            assert(min_step);
            part1 += min_step;

            // Perform the step
            s.erase(min_step);
            for (auto& [step, before] : s) {
                before.erase(min_step);
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        unordered_map<char, unordered_set<char>> s = steps;
        vector<pair<char, int>> worker(5);
        int free = 5;

        // One loop per second
        for (;;) {
            // Check how the workers are doing
            for (auto& [step, left] : worker) {
                if (!left || --left) continue;
                // Worker done with this step
                s.erase(step);
                for (auto& [_, before] : s) {
                    before.erase(step);
                }
                ++free;
            }
            if (s.empty()) break;
            ++part2;
            if (!free) continue;

            // Find work for the free workers
            for (const auto& [step, before] : s) {
                if (!before.empty()) continue;
                // This step has nothing before it
                bool working = false;
                for (auto& [wstep, left] : worker) {
                    if (left && wstep == step) {
                        working = true;
                        break;
                    }
                }
                if (working) continue;
                for (auto& [wstep, left] : worker) {
                    if (left) continue;
                    wstep = step;
                    left = step - 'A' + 61;
                    --free;
                    break;
                }
                if (!free) break;
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
