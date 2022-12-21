/*
 * Problem 20, Advent of Code 2022
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Find a value in a vector and return its index
static ll find_value(const vector<ll>& vec, ll val)
{
    auto iter = find(vec.begin(), vec.end(), val);
    assert(iter != vec.end());
    return iter - vec.begin();
}

// Move the value at index index steps steps
static void move(vector<ll>& file, ll index, ll steps)
{
    if (steps == 0) return;

    // Calculate start and end indices
    ll size = ll(file.size());
    assert(size > 1);
    ll src = index;
    ll dst = (index + steps) % (size - 1);
    if (dst <= 0) dst += size - 1;

    // Move data
    ll value = file[src];
    if (src < dst) {
        copy(file.begin() + src + 1, file.begin() + dst + 1, file.begin() + src);
    } else {
        copy_backward(file.begin() + dst, file.begin() + src, file.begin() + src + 1);
    }
    file[dst] = value;
}

// One mixing round
static void mix(vector<ll>& file, bool first)
{
    ll size = ll(file.size());

    // Build an index file so we can keep track of how each index moves around
    static vector<ll> index_file;
    if (first) {
        index_file.clear();
        for (ll i = 0; i < size; ++i) {
            index_file.push_back(i);
        }
    }

    for (ll i = 0; i < size; ++i) {
        // Find how index i has moved in the file
        ll index = find_value(index_file, i);

        // Mix both the index file and the real file
        move(index_file, index, file[index]);
        move(file, index, file[index]);
    }
}

// Calculate the magic grove coordinate number
static ll calc_answer(const vector<ll>& file)
{
    // Find the index where zero is
    ll index = find_value(file, 0);

    // Add the 3 numbers at the magic offsets
    ll size = ll(file.size());
    ll ret = file[(index + 1000) % size];
    ret += file[(index + 2000) % size];
    ret += file[(index + 3000) % size];

    return ret;
}

int main()
{
    vector<ll> file;
    vector<ll> mixed_file;

    // Parse the input
    string line;
    while (getline(cin, line)) {
        ll value = stoll(line);
        file.push_back(value);
    }
    ll size = ll(file.size());

    // Part 1
    mixed_file = file;
    mix(mixed_file, true);
    cout << "Part 1: " << calc_answer(mixed_file) << endl;

    // Part 2
    mixed_file = file;
    for (ll i = 0; i < size; ++i) {
        // Decryption key
        mixed_file[i] *= 811589153;
    }
    bool first = true;
    for (ll round = 0; round < 10; ++round) {
        mix(mixed_file, first);
        first = false;
    }
    cout << "Part 2: " << calc_answer(mixed_file) << endl;

    return 0;
}
