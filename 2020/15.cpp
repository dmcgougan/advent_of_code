/*
 * Problem 15, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

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

int main()
{
    // Parse input
    string line;
    assert(getline(cin, line));
    vector<string> s = split(line, ",");
    assert(!s.empty());
    static int turn_map[30000000]{};
    int last = atoi(s[0].c_str());
    int turn;
    for (turn = 1; turn < int(s.size()); ++turn) {
        turn_map[last] = turn;
        last = atoi(s[turn].c_str());
    }

    // Solve
    for (; turn < 2020; ++turn) {
        int last_turn = turn_map[last];
        turn_map[last] = turn;
        last = last_turn ? turn - last_turn : 0;
    }
    int part1 = last;
    for (; turn < 30000000; ++turn) {
        int last_turn = turn_map[last];
        turn_map[last] = turn;
        last = last_turn ? turn - last_turn : 0;
    }
    int part2 = last;

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
