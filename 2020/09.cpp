/*
 * Problem 9, Advent of Code 2020
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<int64_t> list;
    for (int64_t n; cin >> n;) {
        list.push_back(n);
    }

    // Solve part 1
    int64_t part1 = -1;
    for (size_t i = 25; part1 < 0 && i < list.size(); ++i) {
        bool found = false;
        for (size_t j = i - 25; !found && j < i; ++j) {
            for (size_t k = j + 1; !found && k < i; ++k) {
                if (list[j] + list[k] == list[i]) found = true;
            }
        }
        if (!found) part1 = list[i];
    }
    assert(part1 >= 0);

    // Solve part 2
    int64_t part2 = -1;
    for (size_t first = 0; first + 1 < list.size(); ++first) {
        int64_t sum = list[first];
        size_t last;
        for (last = first + 1; sum < part1 && last < list.size(); ++last) {
            sum += list[last];
        }
        if (sum == part1) {
            vector<int64_t> s;
            for (size_t i = first; i <= last; ++i) s.push_back(list[i]);
            sort(s.begin(), s.end());
            part2 = s[0] + s[s.size() - 1];
            ;
            break;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
