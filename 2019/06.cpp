/*
 * Problem 6, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, p0 = 0;
    while ((p = s.find(sep, p0)) != string::npos) {
        if (p > p0) ret.emplace_back(s.substr(p0, p - p0));
        p0 = p + sep.size();
    }
    if (p0 < s.size()) ret.emplace_back(s.substr(p0));
    return ret;
}

static unordered_map<string, string> orbits;

static vector<string> path_to_root(const string& obj)
{
    vector<string> path;
    reference_wrapper<const string> cur = obj;
    for (;;) {
        auto iter = orbits.find(cur.get());
        if (iter == orbits.end()) break;
        cur = iter->second;
        path.emplace_back(cur);
    }
    return path;
}

int main()
{
    // Parse input
    string line;
    while (getline(cin, line)) {
        vector<string> s = split(line, ")");
        assert(s.size() == 2);
        assert(orbits.find(s[1]) == orbits.end());
        orbits[s[1]] = s[0];
    }

    // Solve part 1
    int part1 = 0;
    for (const auto& [obj, _] : orbits) {
        part1 += path_to_root(obj).size();
    }

    // Solve part 2
    int part2 = 0;
    vector<string> you_path = path_to_root("YOU");
    vector<string> san_path = path_to_root("SAN");
    for (size_t i = 0; i < you_path.size(); ++i) {
        for (size_t j = 0; j < san_path.size(); ++j) {
            if (you_path[i] == san_path[j]) {
                part2 = i + j;
                goto done;
            }
        }
    }
done:

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
