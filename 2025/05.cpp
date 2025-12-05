/*
 * Problem 5, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using namespace std;
using ll = int64_t;

// Class that holds a set of integers by storing a sequence of ranges
// (reused from my solution of AoC 2022 day 15)
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

    ll count() const
    {
        ll count = 0;
        for (auto [from, to] : ranges) {
            count += to - from + 1;
        }
        return count;
    }

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

    list<pair<ll, ll>> ranges;
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<pair<ll, ll>> fresh;
    vector<ll> ingr;
    string line;
    while (getline(in, line) && !line.empty()) {
        ll from, to;
        // (portability note: use PRId64 to be portable across Linux/Windows)
        assert(sscanf(line.c_str(), "%" PRId64 "-%" PRId64, &from, &to) == 2);
        fresh.push_back({from, to});
    }
    while (getline(in, line)) {
        ingr.push_back(ll(atoll(line.c_str())));
    }

    // Solve part 1
    int part1 = 0;
    {
        for (ll i : ingr) {
            for (auto [from, to] : fresh) {
                if (i >= from && i <= to) {
                    ++part1;
                    break;
                }
            }
        }
    }

    // Solve part 2
    ll part2;
    {
        range_set rs;
        for (auto [from, to] : fresh) {
            rs.insert(from, to);
        }
        part2 = rs.count();
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
