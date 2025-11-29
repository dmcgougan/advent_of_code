/*
 * Problem 4, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
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

// Check if a passport is valid according to part 1 criteria
static bool valid_p1(const unordered_map<string, string>& fields)
{
    static const string exp[] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    for (const string& e : exp) {
        if (fields.find(e) == fields.end()) return false;
    }
    return true;
}

static bool check_int(const unordered_map<string, string>& fields,
                      const string& name,
                      int from,
                      int to)
{
    auto iter = fields.find(name);
    if (iter == fields.end()) return false;
    const string& s = iter->second;
    if (s.size() != 4) return false;
    int num = atoi(s.c_str());
    if (num < from || num > to) return false;
    return true;
}

// Check if a passport is valid according to part 2 criteria
static bool valid_p2(const unordered_map<string, string>& fields)
{
    if (!check_int(fields, "byr", 1920, 2002)) return false;
    if (!check_int(fields, "iyr", 2010, 2020)) return false;
    if (!check_int(fields, "eyr", 2020, 2030)) return false;

    {
        auto iter = fields.find("hgt");
        if (iter == fields.end()) return false;
        const string& s = iter->second;
        int size = s.size();
        if (size < 4 || size > 5) return false;
        int num = atoi(s.c_str());
        if (s[size - 2] == 'c' && s[size - 1] == 'm') {
            if (num < 150 || num > 193) return false;
        } else if (s[size - 2] == 'i' && s[size - 1] == 'n') {
            if (num < 59 || num > 76) return false;
        } else {
            return false;
        }
    }

    {
        auto iter = fields.find("hcl");
        if (iter == fields.end()) return false;
        const string& s = iter->second;
        int size = s.size();
        if (size != 7 || s[0] != '#') return false;
        for (int i = 1; i < size; ++i) {
            if (!(s[i] >= '0' && s[i] <= '9') && !(s[i] >= 'a' && s[i] <= 'f')) return false;
        }
    }

    {
        auto iter = fields.find("ecl");
        if (iter == fields.end()) return false;
        const string& s = iter->second;
        if (!unordered_set<string>{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}.count(s))
            return false;
    }

    {
        auto iter = fields.find("pid");
        if (iter == fields.end()) return false;
        const string& s = iter->second;
        int size = s.size();
        if (size != 9) return false;
        for (int i = 0; i < size; ++i) {
            if (!(s[i] >= '0' && s[i] <= '9')) return false;
        }
    }

    return true;
}

int main()
{
    // Parse input and solve
    string line;
    unordered_map<string, string> fields;
    int part1 = 0;
    int part2 = 0;
    for (bool done = false; !done;) {
        done = !getline(cin, line);
        if (!done && !line.empty()) {
            vector<string> s = split(line, " ");
            for (const string& p : s) {
                vector<string> kv = split(p, ":");
                assert(kv.size() == 2);
                fields[kv[0]] = kv[1];
            }
        }
        if ((done || line.empty()) && !fields.empty()) {
            if (valid_p1(fields)) ++part1;
            if (valid_p2(fields)) ++part2;
            fields.clear();
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
