/*
 * Problem 13, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <iostream>
#include <tuple>
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

static bool is_prime(int v)
{
    for (int i = 2; i * i <= v; ++i) {
        if (v % i == 0) return false;
    }
    return true;
}

static tuple<ll, ll, ll> egcd(ll a, ll b)
{
    if (b == 0) return make_tuple(a, 1, 0);
    auto [gcd, x1, y1] = egcd(b, a % b);
    return make_tuple(gcd, y1, x1 - (a / b) * y1);
}

int main()
{
    // Parse input
    string line;
    assert(getline(cin, line));
    int start = atoi(line.c_str());
    assert(getline(cin, line));
    vector<string> s = split(line, ",");
    vector<pair<int, int>> buses;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != "x") {
            int bus = atoi(s[i].c_str());
            buses.push_back({bus, i});
        }
    }

    // Solve part 1
    int min_diff = INT_MAX;
    int min_bus = 0;
    for (auto [bus, _] : buses) {
        int b;
        for (b = start / bus * bus; b < start; b += bus)
            ;
        int diff = b - start;
        if (diff < min_diff) {
            min_diff = diff;
            min_bus = bus;
        }
    }
    int part1 = min_bus * min_diff;

    // Solve part 2
    // Since all bus IDs are prime we can use the Chinese Remainder Theorem
    ll N = 1;
    for (auto [bus, _] : buses) {
        assert(is_prime(bus));
        N *= bus;
    }
    ll part2 = 0;
    for (auto [mod, rem] : buses) {
        // Negate the remainder
        rem = (mod - rem % mod) % mod;
        auto [gcd, x, y] = egcd(N / mod, mod);
        assert(gcd == 1);
        part2 += rem * (N / mod) * ((x % mod + mod) % mod);
        part2 %= N;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
