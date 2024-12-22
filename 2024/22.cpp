/*
 * Problem 22, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using ll = int64_t;

static inline unsigned get_next(unsigned n)
{
    n = (n ^ (n << 6)) & 0xffffff;
    n = (n ^ (n >> 5)) & 0xffffff;
    n = (n ^ (n << 11)) & 0xffffff;
    return n;
}

int main()
{
    ll part1 = 0;
    unordered_map<uint32_t, int> change_seq_scores;
    unordered_set<uint32_t> change_seq_used;
    unsigned n;
    while (cin >> n) {
        // Represent a change sequence as bits within a uint32_t
        // Each change uses 8 bits of the uint32_t
        uint32_t change_seq = 0;
        int prev_price;
        int price = n % 10;
        size_t i;
        // Fill up the change sequence
        for (i = 0; i < 3; i++) {
            n = get_next(n);
            prev_price = price;
            price = n % 10;
            change_seq = (change_seq << 8) | ((price - prev_price) + 9);
        }
        for (; i < 2000; i++) {
            n = get_next(n);
            prev_price = price;
            price = n % 10;
            // Add a new change to the sequence
            // The oldest one will automatically be shifed out and discarded
            change_seq = (change_seq << 8) | ((price - prev_price) + 9);
            if (change_seq_used.count(change_seq)) continue;
            change_seq_used.insert(change_seq);
            // This change sequence is seen for the first time
            // Add it to the map of scores
            change_seq_scores[change_seq] += price;
        }
        part1 += n;
        change_seq_used.clear();
    }

    // Find the highest scoring change sequence
    int part2 = 0;
    for (auto [change_seq, score] : change_seq_scores) {
        part2 = max(part2, score);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
