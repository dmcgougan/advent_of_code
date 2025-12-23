/*
 * Problem 9, Advent of Code 2018
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using ll = int64_t;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    int players;
    int last;
    {
        string line;
        assert(getline(in, line));
        assert(
            sscanf(line.c_str(), "%d players; last marble is worth %d points", &players, &last) ==
            2);
    }

    // Common
    auto score = [&](int players, int last) -> ll {
        vector<int> next(last + 1);
        vector<int> prev(last + 1);
        vector<ll> score(players);
        int cur = 0;
        next[0] = 0;
        prev[0] = 0;
        for (int m = 1, pl = 0; m <= last; ++m, pl = (pl + 1) % players) {
            if (m % 23) {
                cur = next[cur];
                int n = next[cur];
                next[cur] = m;
                prev[n] = m;
                next[m] = n;
                prev[m] = cur;
                cur = m;
            } else {
                score[pl] += m;
                for (int i = 0; i < 7; i++) {
                    cur = prev[cur];
                }
                score[pl] += cur;
                int p = prev[cur];
                int n = next[cur];
                next[p] = n;
                prev[n] = p;
                cur = n;
            }
        }
        return *std::max_element(score.begin(), score.end());
    };

    // Solve part 1
    ll part1 = score(players, last);

    // Solve part 2
    ll part2 = score(players, last * 100);

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
