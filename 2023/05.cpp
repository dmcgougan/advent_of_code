/*
 * Problem 5, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Class that holds a set of integers by storing a sequence of ranges
class range_set
{
public:
    void insert(ll from, ll to)
    {
        assert(from <= to);
        auto iter = ranges.begin();
        while (iter != ranges.end()) {
            auto& p = *iter;
            if (from < p.first) {
                if (to < p.first) {
                    ranges.insert(iter, make_pair(from, to));
                    break;
                }
                if (to <= p.second) {
                    p.first = from;
                    break;
                }
                iter = ranges.erase(iter);
                continue;
            }
            if (from <= p.second) {
                if (to <= p.second) {
                    break;
                }
                p.second = to;
                break;
            }
            ++iter;
        }
        if (iter == ranges.end()) {
            ranges.push_back(make_pair(from, to));
        }
        merge();
    }

    void convert(ll from, ll to, ll newfrom)
    {
        assert(from <= to);
        auto iter = ranges.begin();
        while (iter != ranges.end()) {
            auto& p = *iter;
            if (from < p.first) {
                if (to < p.first) {
                    return;
                }
                if (to >= p.second) {
                    convertedranges.push_back(
                        make_pair(p.first - from + newfrom, p.second - from + newfrom));
                    iter = ranges.erase(iter);
                    continue;
                }
                convertedranges.push_back(make_pair(p.first - from + newfrom, to - from + newfrom));
                p.first = to + 1;
                return;
            }
            if (from <= p.second) {
                if (to < p.second) {
                    if (from > p.first) {
                        ranges.insert(iter, make_pair(p.first, from - 1));
                    }
                    convertedranges.push_back(make_pair(newfrom, to - from + newfrom));
                    p.first = to + 1;
                    return;
                }
                if (from > p.first) {
                    convertedranges.push_back(make_pair(newfrom, p.second - from + newfrom));
                    p.second = from - 1;
                } else {
                    convertedranges.push_back(
                        make_pair(p.first - from + newfrom, p.second - from + newfrom));
                    iter = ranges.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
        merge();
    }

    void convert_commit()
    {
        for (const pair<ll, ll>& p : convertedranges) {
            insert(p.first, p.second);
        }
        convertedranges.clear();
    }

    ll lowest() { return ranges.front().first; }

private:
    void merge()
    {
        auto iter = ranges.begin();
        while (iter != ranges.end() && next(iter, 1) != ranges.end()) {
            auto& l = *iter;
            auto& r = *next(iter, 1);
            assert(l.first < r.second);
            if (l.second >= r.first) {
                if (r.second > l.second) {
                    r.first = l.first;
                    iter = ranges.erase(iter);
                    continue;
                }
                ranges.erase(next(iter, 1));
                continue;
            }
            ++iter;
        }
    }

public:
    list<pair<ll, ll>> ranges;
    list<pair<ll, ll>> convertedranges;
};

int main()
{
    vector<ll> seeds;
    range_set rseeds;
    vector<vector<array<ll, 3>>> maps;

    // Parse input data
    string line;
    getline(cin, line);
    stringstream ss(line);
    string s;
    ss >> s;
    ll n1;
    ll n2;
    while ((ss >> n1) && (ss >> n2)) {
        seeds.push_back(n1);
        seeds.push_back(n2);
        rseeds.insert(n1, n1 + n2 - 1);
    }
    assert(getline(cin, line));
    for (;;) {
        if (!getline(cin, line)) break;
        vector<array<ll, 3>> map;
        while (getline(cin, line) && !line.empty()) {
            stringstream ss(line);
            array<ll, 3> a;
            ss >> a[0];
            ss >> a[1];
            ss >> a[2];
            map.push_back(a);
        }
        maps.emplace_back(move(map));
    }

    // Part 1
    ll part1 = INT64_MAX;
    for (ll s : seeds) {
        for (const vector<array<ll, 3>>& map : maps) {
            for (const array<ll, 3>& r : map) {
                if (s >= r[1] && s < r[1] + r[2]) {
                    ll sn = r[0] + (s - r[1]);
                    s = sn;
                    break;
                }
            }
        }
        if (s < part1) part1 = s;
    }

    // Part 2
    for (const vector<array<ll, 3>>& map : maps) {
        for (const array<ll, 3>& r : map) {
            rseeds.convert(r[1], r[1] + r[2] - 1, r[0]);
        }
        rseeds.convert_commit();
    }
    ll part2 = rseeds.lowest();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
