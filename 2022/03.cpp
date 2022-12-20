/*
 * Problem 3, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <string>

using namespace std;
using ll = int64_t;

static int prio(char c) { return c >= 'a' ? c - 'a' + 1 : c - 'A' + 27; }

int main()
{
    string line;
    string group[3];
    ll p1_sum = 0;
    ll p2_sum = 0;
    ll ix = 0;

    while (getline(cin, line)) {
        assert(!line.empty() && line.size() % 2 == 0);
        string first = line.substr(0, line.size() / 2);
        string last = line.substr(line.size() / 2);
        for (char c : first) {
            if (last.find(c) != std::string::npos) {
                // Found the item that is in both compartments
                p1_sum += prio(c);
                break;
            }
        }

        group[ix++] = line;
        if (ix == 3) {
            // We now have a group of 3 elves
            ix = 0;
            for (char c : group[0]) {
                if (group[1].find(c) != string::npos && group[2].find(c) != string::npos) {
                    // Found the common item
                    p2_sum += prio(c);
                    break;
                }
            }
        }
    }

    cout << "Part 1: " << p1_sum << endl;
    cout << "Part 2: " << p2_sum << endl;

    return 0;
}
