/*
 * Problem 11, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <map>

using namespace std;
using ll = int64_t;

// Count how many digits a number has
static int digits(ll v)
{
    int count = 0;
    do {
        count++;
        v /= 10;
    } while (v);
    return count;
}

// Count how many stones a specific stone will turn into after the specified number of blinks
static ll count(ll stone, int blinks)
{
    static map<pair<ll, int>, ll> cache;

    // When no blinks are left we only have the one stone
    if (blinks == 0) return 1;

    // Check cache
    auto iter = cache.find(make_pair(stone, blinks));
    if (iter != cache.end()) {
        return (*iter).second;
    }

    // Calculate recursively
    ll result;
    int d;
    if (stone == 0) {
        result = count(1, blinks - 1);
    } else if ((d = digits(stone)) % 2 == 0) {
        ll left = stone;
        ll right = 0;
        ll place = 1;
        for (int i = 0; i < d / 2; i++) {
            right += place * (left % 10);
            place *= 10;
            left /= 10;
        }
        result = count(left, blinks - 1) + count(right, blinks - 1);
    } else {
        result = count(stone * 2024, blinks - 1);
    }

    // Store the result in the cache
    cache[make_pair(stone, blinks)] = result;

    return result;
}

int main()
{
    ll part1 = 0;
    ll part2 = 0;

    ll v;
    while (cin >> v) {
        part1 += count(v, 25);
        part2 += count(v, 75);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
