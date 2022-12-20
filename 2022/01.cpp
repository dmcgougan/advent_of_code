
/*
 * Problem 1, Advent of Code 2022
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const std::string& str, const std::string& sep)
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

int main()
{
    // Parse input
    stringstream cin_ss;
    cin_ss << cin.rdbuf();
    string data = cin_ss.str();
    vector<string> groups = split(data, "\n\n");
    vector<ll> sums;
    for (const string& g : groups) {
        vector<string> v = split(g, "\n");
        ll sum = 0;
        for_each(v.begin(), v.end(), [&sum](string s) { sum += stol(s); });
        sums.push_back(sum);
    }

    // Sort in descending order
    sort(sums.rbegin(), sums.rend());

    cout << "Part 1: " << sums[0] << endl;
    cout << "Part 2: " << sums[0] + sums[1] + sums[2] << endl;

    return 0;
}
