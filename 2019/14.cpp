/*
 * Problem 14, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
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

int main()
{
    // Parse input
    unordered_map<string, pair<int, vector<pair<int, string>>>> chems;
    for (string line; getline(cin, line);) {
        vector<string> s = split(line, " => ");
        assert(s.size() == 2);

        // Get the output chemical
        vector<string> result = split(s[1], " ");
        assert(result.size() == 2);
        auto& [count, input] = chems[move(result[1])];
        count = atoi(result[0].c_str());

        // Get the list of input chemicals
        for (const string& inp : split(s[0], ", ")) {
            auto& [count, name] = input.emplace_back();
            vector<string> inp_s = split(inp, " ");
            assert(inp_s.size() == 2);
            count = atoi(inp_s[0].c_str());
            name = move(inp_s[1]);
        }
    }

    // Solve part 1
    unordered_map<string, int64_t> stash;
    auto num_ore = [&](auto& self, string output, int64_t need) -> int64_t {
        auto iter = chems.find(output);
        assert(iter != chems.end());
        const auto& [output_count, inputs] = iter->second;
        // How many times do we need to run the reaction
        int64_t times = (need + output_count - 1) / output_count;
        int64_t ore_count = 0;
        // Calculate how many ore we need to produce the ingredients
        for (const auto& [input_count, input] : inputs) {
            if (input == "ORE") {
                ore_count += times * input_count;
                continue;
            }
            int64_t have = stash[input];
            if (have >= times * input_count) {
                // We already have this in our stash
                stash[input] -= times * input_count;
            } else {
                // Ran out; need to run more reactions
                stash[input] = 0;
                ore_count += self(self, input, times * input_count - have);
            }
        }
        int64_t got = times * output_count;
        assert(got >= need);
        // Put surplus in our stash
        if (got > need) stash[output] += got - need;
        return ore_count;
    };
    int64_t part1 = num_ore(num_ore, "FUEL", 1);

    // Solve part 2
    stash.clear();
    int64_t fuel = 0;
    int64_t ore = 1000000000000;
    for (;;) {
        int64_t produce = ore >= part1 ? ore / part1 : 1;
        int64_t consumed = num_ore(num_ore, "FUEL", produce);
        if (ore < consumed) break;
        fuel += produce;
        ore -= consumed;
    }
    int64_t part2 = fuel;

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
