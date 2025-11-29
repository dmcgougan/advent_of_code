/*
 * Problem 22, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <cstdio>
#include <iostream>
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

// Represents a cube
struct cube
{
    int x1, x2;
    int y1, y2;
    int z1, z2;

    ll volume() const { return ll(x2 - x1) * (y2 - y1) * (z2 - z1); }

    bool intersect(const cube& c, cube& r) const
    {
        r.x1 = max(x1, c.x1);
        r.x2 = min(x2, c.x2);
        r.y1 = max(y1, c.y1);
        r.y2 = min(y2, c.y2);
        r.z1 = max(z1, c.z1);
        r.z2 = min(z2, c.z2);
        return r.x1 < r.x2 && r.y1 < r.y2 && r.z1 < r.z2;
    }
};

// Store a set of cubes
class cube_set
{
    vector<cube> pos;
    vector<cube> neg;

    void add_remove(const cube& c)
    {
        vector<cube> npos;
        vector<cube> nneg;
        for (const auto& p : pos) {
            cube is;
            if (c.intersect(p, is)) nneg.push_back(is);
        }
        for (const auto& n : neg) {
            cube is;
            if (c.intersect(n, is)) npos.push_back(is);
        }
        pos.insert(pos.end(), npos.begin(), npos.end());
        neg.insert(neg.end(), nneg.begin(), nneg.end());
    }

public:
    // Add a cube
    void add(const cube& c)
    {
        add_remove(c);
        pos.push_back(c);
    }

    // Remove a cube
    void remove(const cube& c) { add_remove(c); }

    // Get volume of cubes
    ll volume() const
    {
        ll v = 0;
        for (const cube& p : pos) v += p.volume();
        for (const cube& n : neg) v -= n.volume();
        return v;
    }
};

int main()
{
    // Parse input
    string line;
    cube_set part1;
    cube_set part2;
    while (getline(cin, line)) {
        vector<string> s = split(line, " ");
        assert(s.size() == 2);
        cube c;
        assert(sscanf(s[1].c_str(),
                      "x=%d..%d,y=%d..%d,z=%d..%d",
                      &c.x1,
                      &c.x2,
                      &c.y1,
                      &c.y2,
                      &c.z1,
                      &c.z2) == 6);
        ++c.x2;
        ++c.y2;
        ++c.z2;
        bool on = (s[0] == "on");
        if (c.x1 >= -50 && c.x2 <= 51 && c.y1 >= -50 && c.y2 <= 51 && c.z1 >= -50 && c.z2 <= 51) {
            on ? part1.add(c) : part1.remove(c);
        }
        on ? part2.add(c) : part2.remove(c);
    }

    cout << "Part 1: " << part1.volume() << endl;
    cout << "Part 2: " << part2.volume() << endl;

    return 0;
}
