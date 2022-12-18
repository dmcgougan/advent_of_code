/*
 * Problem 15, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
vector<string>
split(const std::string& str, const std::string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    assert(!sep.empty());
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.emplace_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.emplace_back(move(s));
    }
    return result;
}

// Class that holds a set of integers by storing a sequence of ranges
class range_set {
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

    void remove(ll from, ll to)
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
                    iter = ranges.erase(iter);
                    continue;
                }
                p.first = to + 1;
                return;
            }
            if (from <= p.second) {
                if (to < p.second) {
                    if (from > p.first) {
                        ranges.insert(iter, make_pair(p.first, from - 1));
                    }
                    p.first = to + 1;
                    return;
                }
                if (from > p.first) {
                    p.second = from - 1;
                } else {
                    iter = ranges.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
        merge();
    }

    bool contains(ll value)
    {
        for (auto p : ranges) {
            if (value < p.first)
                return false;
            if (value <= p.second)
                return true;
        }
        return false;
    }

    string to_string() const
    {
        string s;
        for (auto p : ranges) {
            s += '(';
            s += std::to_string(p.first);
            s += '-';
            s += std::to_string(p.second);
            s += ')';
        }
        return s;
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

public:
    list<pair<ll, ll>> ranges;
};

// A sensor reading
struct reading {
    ll sx;
    ll sy;
    ll bx;
    ll by;
    ll dist() const { return abs(sx - bx) + abs(sy - by); }

    // range of x coordinates that can not have the distress beacon at the
    // specified y coordinate
    bool any_xrange_at_y(ll y) const { return abs(y - sy) <= dist(); }
    pair<ll, ll> xrange_at_y(ll y) const
    {
        ll d = dist();
        ll yd = abs(y - sy);
        return make_pair(sx - d + yd, sx + d - yd);
    }
};

int main()
{
    vector<reading> readings;
    set<pair<ll, ll>> beacons;
    ll avg_sx = 0;

    // Parse input
    string data;
    stringstream cin_ss;
    cin_ss << cin.rdbuf();
    data = cin_ss.str();
    vector<string> lines = split(data, "\n");
    for (string& line : lines) {
        vector<string> words = split(line, " ");
        reading r;
        stringstream(words[2].substr(2)) >> r.sx;
        stringstream(words[3].substr(2)) >> r.sy;
        stringstream(words[8].substr(2)) >> r.bx;
        stringstream(words[9].substr(2)) >> r.by;
        beacons.insert(make_pair(r.bx, r.by));
        avg_sx += r.sx;
        readings.push_back(r);
    }
    avg_sx /= readings.size();
    bool test_data = avg_sx < 100;

    // Part 1
    ll y = test_data ? 10 : 2000000;
    ll count = 0;
    for (ll x = -6000000; x <= 6000000; ++x) {
        for (const reading& r : readings) {
            if (abs(x - r.sx) + abs(y - r.sy) <= r.dist() && beacons.count(make_pair(x, y)) == 0) {
                count++;
                break;
            }
        }
    }
    cout << "Part 1: " << count << endl;

    // Part 2
    // For each y:
    //   Maintain a set of x ranges that could hold the distress beacon.
    //   Remove from this set all x ranges that the sensor readings exclude.
    // Find the y coordinate where this set is not empty.
    range_set rset;
    ll to = test_data ? 20 : 4000000;
    for (ll y = 0; y <= to; ++y) {
        rset.insert(0, to);
        for (const reading& r : readings) {
            if (r.any_xrange_at_y(y)) {
                auto p = r.xrange_at_y(y);
                rset.remove(p.first, p.second);
                if (rset.ranges.empty())
                    break;
            }
        }
        if (!rset.ranges.empty()) {
            // Found a position that is not excluded by the readings
            ll freq = rset.ranges.front().first * 4000000 + y;
            cout << "Part 2: " << freq << endl;
        }
    }

    return 0;
}
