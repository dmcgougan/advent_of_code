/*
 * Problem 9, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <set>
#include <string>

using namespace std;
using ll = int64_t;

int main()
{
    string line;
    set<pair<ll, ll>> tailTrail1;
    set<pair<ll, ll>> tailTrail9;

    ll x[10] = {0};
    ll y[10] = {0};

    tailTrail1.insert(make_pair(0, 0));
    tailTrail9.insert(make_pair(0, 0));

    while (getline(cin, line)) {
        // Get direction
        ll dx = 0, dy = 0;
        char dir = line[0];
        switch (dir) {
        case 'R':
            dx = 1;
            break;
        case 'L':
            dx = -1;
            break;
        case 'D':
            dy = 1;
            break;
        case 'U':
            dy = -1;
            break;
        default:
            assert(false);
        }

        // Get count
        char* end;
        ll count = strtol(line.substr(2).c_str(), &end, 10);
        assert(*end == '\0');

        // Move
        for (ll i = 0; i < count; i++) {
            // Move head
            x[0] += dx;
            y[0] += dy;

            // Process the tail
            for (ll j = 1; j < 10; j++) {
                // Check if we need to move this tail part
                if (abs(x[j] - x[j - 1]) > 1 || abs(y[j] - y[j - 1]) > 1) {
                    // Yes, move the tail
                    if (x[j] != x[j - 1]) {
                        x[j] += (x[j] < x[j - 1] ? 1 : -1);
                    }
                    if (y[j] != y[j - 1]) {
                        y[j] += (y[j] < y[j - 1] ? 1 : -1);
                    }
                }
            }

            // Record the tail trail
            tailTrail1.insert(make_pair(x[1], y[1]));
            tailTrail9.insert(make_pair(x[9], y[9]));
        }
    }

    cout << "Part 1: " << tailTrail1.size() << endl;
    cout << "Part 2: " << tailTrail9.size() << endl;

    return 0;
}
