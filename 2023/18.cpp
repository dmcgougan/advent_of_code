/*
 * Problem 18, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using ll = int64_t;

static ll solve(const vector<pair<char, int>>& plan)
{
    // Get the vertices of the bounding polygon
    vector<array<ll, 2>> p;
    ll boundary = 0;
    ll r = 0;
    ll c = 0;
    for (auto [ch, len] : plan) {
        int dr = (ch == 'U' ? -1 : (ch == 'D' ? 1 : 0));
        int dc = (ch == 'L' ? -1 : (ch == 'R' ? 1 : 0));
        r += dr * len;
        c += dc * len;
        p.push_back({r, c});
        boundary += len;
    }

    // Calculate the area of the polygon
    // http://en.wikipedia.org/wiki/Shoelace_formula
    ll area = 0;
    for (size_t i = 0; i < p.size(); ++i) {
        size_t next = i + 1 < p.size() ? i + 1 : 0;
        size_t prev = i > 0 ? i - 1 : p.size() - 1;
        area += p[i][0] * (p[next][1] - p[prev][1]);
    }
    area = abs(area) / 2;

    // The area is for a polygon where the vertices are in the centers of the tiles
    // Compensate and get the area bounded by the outsides of the tiles
    return area + boundary / 2 + 1;
}

static int hexdigit(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    return ch - 'a' + 10;
}

int main()
{
    static const char hex2dir[] = "RDLU";

    // Parse input data
    vector<pair<char, int>> plan1;
    vector<pair<char, int>> plan2;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        char ch;
        ss >> ch;
        int len;
        ss >> len;
        string hexdata;
        ss >> hexdata;
        int hexlen = 0;
        for (int i = 2; i <= 6; ++i) hexlen = (hexlen << 4) | hexdigit(hexdata[i]);
        plan1.push_back({ch, len});
        plan2.push_back({hex2dir[hexdata[7] - '0'], hexlen});
    }

    // Part 1
    ll part1 = solve(plan1);

    // Part 2
    ll part2 = solve(plan2);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
