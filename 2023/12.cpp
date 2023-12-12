/*
 * Problem 12, Advent of Code 2023
 * Danjel McGougan
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static vector<int> grp;

static ll count_arr(string s, int grp_first, int grp_len, int consec)
{
    for (;;) {
        if (s.empty()) {
            if (consec == 0) return grp_len == 0 ? 1 : 0;
            return grp_len == 1 && grp[grp_first] == consec ? 1 : 0;
        }
        if (s.size() == 1 && s[0] == '?') {
            if (grp_len > 1) return 0;
            if (grp_len == 0) return consec == 0 ? 1 : 0;
            return (grp[grp_first] == consec || grp[grp_first] == consec + 1) ? 1 : 0;
        }

        int grphash = 0;
        for (int i = grp_first; i < grp_first + grp_len; ++i) grphash += grp[i];
        if (int(s.size()) + consec < grphash) return 0;
        int shash = count(s.begin(), s.end(), '#');
        if (shash + consec > grphash) return 0;

        if (s.back() == '#') {
            ++consec;
            s.pop_back();
            if (!grp_len || consec > grp[grp_first + grp_len - 1]) return 0;
        } else if (s.back() == '.') {
            if (consec > 0) {
                if (!grp_len || grp[grp_first + grp_len - 1] != consec) return 0;
                --grp_len;
                consec = 0;
            }
            s.pop_back();
        } else {
            // Split the string at ? or . near midpoint
            ll count = 0;
            size_t mpos = s.size() / 2;
            while (s[mpos] == '#') ++mpos;
            if (mpos + 1 == s.size()) {
                mpos = s.size() / 2;
                while (s[mpos] == '#' && mpos > 0) --mpos;
                if (mpos == 0 || mpos + 1 == s.size()) {
                    // Corner case when string can not be split
                    s.back() = '#';
                    count += count_arr(s, grp_first, grp_len, consec);
                    s.back() = '.';
                    count += count_arr(s, grp_first, grp_len, consec);
                    return count;
                }
            }
            // Calculate the combinations for each side of the midpoint
            string left = s.substr(0, mpos);
            string right = s.substr(mpos + 1);
            for (int i = 0; i <= grp_len; ++i) {
                ll lc = count_arr(left, grp_first, grp_len - i, 0);
                ll rc = lc ? count_arr(right, grp_first + grp_len - i, i, consec) : 0;
                count += lc * rc;
            }
            // Replace the midpoint ? with # and add those combinations
            if (s[mpos] == '?') {
                s[mpos] = '#';
                count += count_arr(s, grp_first, grp_len, consec);
            }
            return count;
        }
    }
}

int main()
{
    // Parse input data
    static vector<pair<string, vector<int>>> rec;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string s;
        ss >> s;
        int c;
        vector<int> l;
        while (ss >> c) {
            l.push_back(c);
            if (ss.peek() == ',') ss.ignore();
        }
        rec.push_back(make_pair(s, l));
    }

    ll part1 = 0;
    ll part2 = 0;
    for (const auto& p : rec) {
        // Part 1
        grp = p.second;
        part1 += count_arr(p.first, 0, grp.size(), 0);
        // Part 2
        grp.clear();
        for (int i = 0; i < 5; i++) {
            for (int e : p.second) {
                grp.push_back(e);
            }
        }
        part2 += count_arr(p.first + "?" + p.first + "?" + p.first + "?" + p.first + "?" + p.first,
                           0,
                           grp.size(),
                           0);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
