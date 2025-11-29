/*
 * Problem 14, Advent of Code 2021
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>
#include <map>

using namespace std;

static array<int64_t, 26> operator-(const array<int64_t, 26>& arg1, const array<int64_t, 26>& arg2)
{
    array<int64_t, 26> res;
    for (size_t i = 0; i < 26; ++i) {
        res[i] = arg1[i] - arg2[i];
    }
    return res;
}

static array<int64_t, 26>& operator+=(array<int64_t, 26>& lhs, const array<int64_t, 26>& rhs)
{
    for (size_t i = 0; i < 26; ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

static map<tuple<char, char, int>, array<int64_t, 26>> cache;

static void recursive_count(const map<pair<char, char>, char>& rules,
                            array<int64_t, 26>& hist,
                            char c0,
                            char c1,
                            int steps)
{
    if (steps == 0) return;
    auto citer = cache.find({c0, c1, steps});
    if (citer != cache.end()) {
        hist += citer->second;
        return;
    }
    array<int64_t, 26> before(hist);
    auto iter = rules.find({c0, c1});
    if (iter != rules.end()) {
        ++hist[iter->second - 'A'];
        recursive_count(rules, hist, c0, iter->second, steps - 1);
        recursive_count(rules, hist, iter->second, c1, steps - 1);
    }
    cache[{c0, c1, steps}] = hist - before;
}

static int64_t count(const string& s, const map<pair<char, char>, char>& rules, int steps)
{
    array<int64_t, 26> hist{};
    for (char c : s) {
        ++hist[c - 'A'];
    }
    char c0 = s[0];
    for (size_t i = 1; i < s.size(); ++i) {
        char c1 = s[i];
        recursive_count(rules, hist, c0, c1, steps);
        c0 = c1;
    }
    int64_t min_c = INT64_MAX;
    int64_t max_c = INT64_MIN;
    for (int64_t c : hist) {
        if (c) {
            min_c = min(c, min_c);
            max_c = max(c, max_c);
        }
    }
    return max_c - min_c;
}

int main()
{
    string s;
    getline(cin, s);
    string line;
    getline(cin, line);
    map<pair<char, char>, char> rules;
    while (getline(cin, line)) {
        char c0, c1, r;
        assert(sscanf(line.c_str(), "%c%c -> %c", &c0, &c1, &r) == 3);
        rules[{c0, c1}] = r;
    }

    int64_t part1 = count(s, rules, 10);
    int64_t part2 = count(s, rules, 40);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
