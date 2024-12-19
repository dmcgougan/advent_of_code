/*
 * Problem 19, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static map<size_t, ll> cache;

// Recursively check how many ways the patterns can match the given design
// Cache the results to prevent recalculating stuff (memoization)
static ll check(const string& design, const vector<string>& patterns, size_t pos)
{
    if (pos == design.size()) return 1;
    auto iter = cache.find(pos);
    if (iter != cache.end()) return iter->second;
    ll ways = 0;
    for (const string& p : patterns) {
        if (pos + p.size() <= design.size() && design.compare(pos, p.size(), p) == 0) {
            ways += check(design, patterns, pos + p.size());
        }
    }
    cache[pos] = ways;
    return ways;
}

int main()
{
    string line;
    vector<string> patterns;
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        while (ss.good()) {
            string s;
            getline(ss, s, ',');
            while (!s.empty() && isspace(s[0])) s = s.substr(1);
            patterns.emplace_back(move(s));
        }
    }
    vector<string> designs;
    while (getline(cin, line)) {
        designs.emplace_back(move(line));
    }

    int part1 = 0;
    ll part2 = 0;
    for (const string& d : designs) {
        // The cache needs to be cleared before each new design
        cache.clear();
        ll ways = check(d, patterns, 0);
        if (ways) part1++;
        part2 += ways;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
