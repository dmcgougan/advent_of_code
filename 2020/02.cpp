/*
 * Problem 2, Advent of Code 2020
 * Danjel McGougan
 */

#include <algorithm>
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
    string line;
    int part1 = 0;
    int part2 = 0;
    while (getline(cin, line)) {
        // Parse line
        vector<string> s = split(line, " ");
        assert(s.size() == 3 && s[1].size() == 2);
        int from, to;
        assert(sscanf(s[0].c_str(), "%d-%d", &from, &to) == 2);
        assert(from > 0 && to > 0);
        char ch = s[1][0];
        const string& password = s[2];
        int size = password.size();

        // Part 1
        int c = count(password.begin(), password.end(), ch);
        if (c >= from && c <= to) part1++;

        // Part 2
        c = (from <= size && password[from - 1] == ch ? 1 : 0);
        c += (to <= size && password[to - 1] == ch ? 1 : 0);
        if (c == 1) part2++;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
