/*
 * Problem 6, Advent of Code 2021
 * Danjel McGougan
 */

#include <iostream>
#include <map>

using namespace std;
using ll = int64_t;

static ll count(int state, int days)
{
    static map<pair<int, int>, ll> cache;
    if (days == 0) return 1;
    auto iter = cache.find({state, days});
    if (iter != cache.end()) return iter->second;
    ll ret;
    if (state > 0) {
        ret = count(state - 1, days - 1);
    } else {
        ret = count(6, days - 1) + count(8, days - 1);
    }
    cache[{state, days}] = ret;
    return ret;
}

int main()
{
    ll part1 = 0;
    ll part2 = 0;
    int v;
    while (cin >> v) {
        char c;
        cin >> c;

        part1 += count(v, 80);
        part2 += count(v, 256);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
