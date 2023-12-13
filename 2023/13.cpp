/*
 * Problem 13, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = int64_t;

// Calculate the reflection value
static int count_h(const vector<string>& pat, int avoid)
{
    unordered_set<string> uni;
    vector<unordered_set<string>::iterator> upat;
    uni.reserve(pat.size());
    for (string s : pat) {
        auto iter = uni.insert(s).first;
        upat.push_back(iter);
    }
    for (size_t i = 0; i + 1 < upat.size(); ++i) {
        bool refl = true;
        for (size_t j = 0; i >= j && i + j + 1 < upat.size(); ++j) {
            if (upat[i - j] != upat[i + j + 1]) {
                refl = false;
                break;
            }
        }
        if (refl && int(i + 1) != avoid) return i + 1;
    }
    return 0;
}

// Turn rows into columns and columns into rows
static vector<string> zip(const vector<string>& pat)
{
    int rows = pat.size();
    int cols = pat[0].size();
    vector<string> ret(vector<string>(cols, string(rows, ' ')));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            ret[c][r] = pat[r][c];
        }
    }
    return ret;
}

int main()
{
    // Parse input data
    string line;
    vector<vector<string>> patterns;
    vector<string> pattern;
    while (getline(cin, line)) {
        if (line.empty()) {
            patterns.emplace_back(move(pattern));
            pattern.clear();
        } else {
            pattern.push_back(line);
        }
    }
    patterns.push_back(pattern);

    ll part1 = 0;
    ll part2 = 0;
    for (auto p : patterns) {
        // Part 1
        int h = count_h(p, 0);
        int v = count_h(zip(p), 0);
        part1 += 100 * h + v;

        // Part 2
        int rows = p.size();
        int cols = p[0].size();
        bool found = false;
        for (int r = 0; !found && r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                p[r][c] = (p[r][c] == '#' ? '.' : '#');
                int nh = count_h(p, h);
                int nv = count_h(zip(p), v);
                p[r][c] = (p[r][c] == '#' ? '.' : '#');
                int nc = 100 * nh + nv;
                if (!nc) continue;
                part2 += nc;
                found = true;
                break;
            }
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
