/*
 * Problem 22, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;
using mystr = basic_string<int8_t>;

static inline unsigned get_next(unsigned n)
{
    n ^= (n << 6);
    n &= 0xffffff;
    n ^= (n >> 5);
    n &= 0xffffff;
    n ^= (n << 11);
    n &= 0xffffff;
    return n;
}

int main()
{
    // Slow brute force solution that takes about 6 minutes on my computer.
    // I will revisit this one to come up with a better solution.

    ll part1 = 0;
    vector<pair<mystr, mystr>> seqs;
    unsigned n;
    while (cin >> n) {
        // Precompute price and price change sequences
        mystr seq;
        seq.reserve(2000);
        mystr price_seq;
        price_seq.reserve(2000);
        int prev_price;
        int price = n % 10;
        for (size_t i = 0; i < 2000; i++) {
            prev_price = price;
            n = get_next(n);
            price = n % 10;
            seq += (int8_t)(price - prev_price);
            price_seq += (int8_t)price;
        }
        seqs.emplace_back(make_pair(move(seq), move(price_seq)));
        part1 += n;
    }

    // Loop through all possible change lists
    int part2 = 0;
    mystr c(4, 0);
    for (c[0] = -9; c[0] <= 9; ++c[0]) {
        for (c[1] = -9; c[1] <= 9; ++c[1]) {
            for (c[2] = -9; c[2] <= 9; ++c[2]) {
                for (c[3] = -9; c[3] <= 9; ++c[3]) {
                    // Find the candidate changes in the precomputed sequence
                    int count = 0;
                    for (const auto& [seq, price] : seqs) {
                        size_t ix = seq.find(c);
                        if (ix != mystr::npos) {
                            count += (int)price[ix + 3];
                        }
                    }
                    part2 = max(part2, count);
                }
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
