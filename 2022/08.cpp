/*
 * Problem 8, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

int main()
{
    string line;
    vector<string> grid;

    while (getline(cin, line)) {
        grid.push_back(line);
    }

    ll visible = 0;
    for (ll y = 0; y < ll(grid.size()); ++y) {
        for (ll x = 0; x < ll(grid[y].size()); ++x) {
            char h = grid[y][x];
            assert(isdigit(h));

            // Look up
            ll i;
            for (i = y - 1; i >= 0; --i) {
                if (grid[i][x] >= h) {
                    break;
                }
            }
            if (i < 0) {
                // Visible
                visible++;
                continue;
            }

            // Look down
            for (i = y + 1; i < ll(grid.size()); ++i) {
                if (grid[i][x] >= h) {
                    break;
                }
            }
            if (i == ll(grid.size())) {
                // Visible
                visible++;
                continue;
            }

            // Look left
            for (i = x - 1; i >= 0; --i) {
                if (grid[y][i] >= h) {
                    break;
                }
            }
            if (i < 0) {
                // Visible
                visible++;
                continue;
            }

            // Look right
            for (i = x + 1; i < ll(grid[y].size()); ++i) {
                if (grid[y][i] >= h) {
                    break;
                }
            }
            if (i == ll(grid[y].size())) {
                // Visible
                visible++;
            }
        }
    }

    cout << "Part 1: " << visible << endl;

    ll max_score = 0;
    for (ll y = 0; y < ll(grid.size()); ++y) {
        for (ll x = 0; x < ll(grid[y].size()); ++x) {
            char h = grid[y][x];
            assert(isdigit(h));

            // Look up
            ll up = 0;
            ll i;
            for (i = y - 1; i >= 0; --i) {
                up++;
                if (grid[i][x] >= h) {
                    break;
                }
            }

            // Look down
            ll down = 0;
            for (i = y + 1; i < ll(grid.size()); ++i) {
                down++;
                if (grid[i][x] >= h) {
                    break;
                }
            }

            // Look left
            ll left = 0;
            for (i = x - 1; i >= 0; --i) {
                left++;
                if (grid[y][i] >= h) {
                    break;
                }
            }

            // Look right
            ll right = 0;
            for (i = x + 1; i < ll(grid[y].size()); ++i) {
                right++;
                if (grid[y][i] >= h) {
                    break;
                }
            }

            ll score = up * down * left * right;
            if (score > max_score) {
                max_score = score;
            }
        }
    }

    cout << "Part 2: " << max_score << endl;

    return 0;
}
