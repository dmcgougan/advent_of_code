/*
 * Problem 7, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
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

static unordered_map<string, vector<pair<int, string>>> bags;

// Check if a bag contains some other bag
static bool contains(const string& outer, const string& inner)
{
    auto iter = bags.find(outer);
    if (iter == bags.end()) return false;
    const auto& v = iter->second;
    for (const auto& [count, name] : v) {
        if (name == inner || contains(name, inner)) return true;
    }
    return false;
}

// Count how many bags a bag contains
static int bag_count(const string& outer)
{
    auto iter = bags.find(outer);
    if (iter == bags.end()) return 0;
    const auto& v = iter->second;
    int c = 0;
    for (const auto& [count, name] : v) {
        c += count + count * bag_count(name);
    }
    return c;
}

int main()
{
    // Parse input
    string line;
    while (getline(cin, line)) {
        vector<string> s = split(line, " ");
        assert(s.size() >= 7);
        string bag = s[0] + ' ' + s[1];
        if (s.size() == 7) continue;
        auto& v = bags[bag];
        for (size_t i = 4; i < s.size(); i += 4) {
            v.push_back({atoi(s[i].c_str()), s[i + 1] + ' ' + s[i + 2]});
        }
    }

    // Solve part 1
    int part1 = 0;
    for (const auto& [bag, v] : bags) {
        if (contains(bag, "shiny gold")) ++part1;
    }

    // Solve part 2
    int part2 = bag_count("shiny gold");

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
