/*
 * Problem 11, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    unordered_map<string, vector<string>> adj;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        string s;
        assert(ss >> s && s.size());
        auto& v = adj[s.substr(0, s.size() - 1)];
        while (ss >> s) v.push_back(std::move(s));
    }
    assert(!adj.empty());

    // Common
    unordered_map<string, ll> cache;
    auto dfs = [&](auto self, const string& start, const string& end) -> ll {
        if (start == end) return 1;
        auto it = cache.find(start);
        if (it != cache.end()) return it->second;
        ll count = 0;
        for (const string& next : adj[start]) {
            count += self(self, next, end);
        }
        cache.emplace(start, count);
        return count;
    };
    auto paths = [&](const string& start, const string& end) -> ll {
        cache.clear();
        return dfs(dfs, start, end);
    };

    // Solve part 1
    ll part1 = 0;
    {
        part1 = paths("you", "out");
    }

    // Solve part 2
    ll part2 = 0;
    {
        // Check both ways of getting to "out" passing both "dac" and "fft"
        part2 = paths("svr", "fft") * paths("fft", "dac") * paths("dac", "out") +
            paths("svr", "dac") * paths("dac", "fft") * paths("fft", "out");
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
