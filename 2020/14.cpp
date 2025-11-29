/*
 * Problem 14, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& str, const string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.push_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.push_back(move(s));
    }
    return result;
}

int main()
{
    // Parse input
    string line;
    unordered_map<ll, ll> mem1;
    unordered_map<ll, ll> mem2;
    string mask;
    ll set_mask = 0;
    ll clr_mask = 0;
    int xcount = 0;
    while (getline(cin, line)) {
        vector<string> s = split(line, " = ");
        assert(s.size() == 2);
        if (s[0] == "mask") {
            assert(s[1].size() == 36);
            set_mask = 0;
            clr_mask = 0;
            xcount = 0;
            for (ll i = 0, b = (ll(1) << 35); i < 36; ++i, b >>= 1) {
                if (s[1][i] == '0') clr_mask |= b;
                if (s[1][i] == '1') set_mask |= b;
                if (s[1][i] == 'X') ++xcount;
            }
            mask = move(s[1]);
        } else {
            assert(s[0].size() >= 6);
            ll addr = atoll(s[0].c_str() + 4);
            ll value = atoll(s[1].c_str());
            mem1[addr] = (value | set_mask) & ~clr_mask;
            // Go through all possible values of the X bits
            for (ll m = 0; m < (1 << xcount); ++m) {
                ll naddr = addr;
                for (ll i = 0, b = (ll(1) << 35), bm = 1; i < 36; ++i, b >>= 1) {
                    if (mask[i] == '1') naddr |= b;
                    if (mask[i] == 'X') {
                        (m & bm) ? (naddr |= b) : (naddr &= ~b);
                        bm <<= 1;
                    }
                }
                mem2[naddr] = value;
            }
        }
    }

    // Solve part 1
    ll part1 = 0;
    for (auto [_, value] : mem1) {
        part1 += value;
    }

    // Solve part 2
    ll part2 = 0;
    for (auto [_, value] : mem2) {
        part2 += value;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
