/*
 * Problem 3, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<string> bat;
    for (string line; getline(in, line);) {
        bat.emplace_back(std::move(line));
    }

    // Common
    auto max_jolts = [](const string& bat, int len) -> int64_t {
        unordered_map<int, int64_t> cache;
        const string& s = bat;
        auto recurse = [&](auto& self, int cur, int len) -> int64_t {
            // Check end condition
            if (len == 0 || cur + len > int(s.size())) return 0;

            // Check cache
            auto it = cache.find(cur << 4 | len);
            if (it != cache.end()) return it->second;

            // Calculate 10^(len - 1)
            int64_t dvalue = 1;
            for (int i = 0; i < len - 1; ++i) dvalue *= 10;

            // Check both options: use the current digit or skip it
            int64_t r = max((s[cur] - '0') * dvalue + self(self, cur + 1, len - 1),
                            self(self, cur + 1, len));

            // Save the result to the cache (memoization)
            cache[cur << 4 | len] = r;
            return r;
        };
        return recurse(recurse, 0, len);
    };

    // Solve part 1
    int64_t part1 = 0;
    {
        for (const string& b : bat) {
            part1 += max_jolts(b, 2);
        }
    }

    // Solve part 2
    int64_t part2 = 0;
    {
        for (const string& b : bat) {
            part2 += max_jolts(b, 12);
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
