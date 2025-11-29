/*
 * Problem 16, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>
#include <tuple>
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
    string line;
    vector<tuple<string, int, int, int, int>> rules;
    while (getline(cin, line) && !line.empty()) {
        vector<string> s = split(line, ": ");
        assert(s.size() == 2);
        string name = s[0];
        s = split(s[1], " or ");
        assert(s.size() == 2);
        auto& [n, from0, to0, from1, to1] = rules.emplace_back();
        n = name;
        assert(sscanf(s[0].c_str(), "%d-%d", &from0, &to0) == 2);
        assert(sscanf(s[1].c_str(), "%d-%d", &from1, &to1) == 2);
    }
    assert(getline(cin, line) && line.find("your") != string::npos);
    assert(getline(cin, line));
    vector<int> my_ticket;
    for (const string& s : split(line, ",")) my_ticket.push_back(atoi(s.c_str()));
    assert(getline(cin, line) && line.empty());
    assert(getline(cin, line) && line.find("nearby") != string::npos);
    vector<vector<int>> tickets;
    while (getline(cin, line)) {
        auto& v = tickets.emplace_back();
        for (const string& s : split(line, ",")) v.push_back(atoi(s.c_str()));
        assert(tickets.size() == 1 || v.size() == tickets[0].size());
    }

    // Solve part 1
    int part1 = 0;
    for (auto iter = tickets.begin(); iter != tickets.end();) {
        bool remove = false;
        for (int v : *iter) {
            bool valid = false;
            for (const auto& [_, from0, to0, from1, to1] : rules) {
                if ((v >= from0 && v <= to0) || (v >= from1 && v <= to1)) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                part1 += v;
                remove = true;
            }
        }
        remove ? iter = tickets.erase(iter) : ++iter;
    }

    // Solve part 2
    assert(rules.size() == tickets[0].size());
    int fields = rules.size();
    // Determine which rules are compatible with which fields
    vector<vector<bool>> invalid(rules.size(), vector<bool>(rules.size(), false));
    for (int rule = 0; rule < fields; ++rule) {
        const auto& [_, from0, to0, from1, to1] = rules[rule];
        for (int field = 0; field < fields; ++field) {
            int vcount = 0;
            for (const vector<int>& t : tickets) {
                int v = t[field];
                if ((v >= from0 && v <= to0) || (v >= from1 && v <= to1)) vcount++;
            }
            if (vcount < int(tickets.size())) {
                invalid[field][rule] = true;
            }
        }
    }
    // Assign rules to each field
    vector<int> rule_field(fields, -1);
    vector<bool> field_done(fields, false);
    for (int i = 0; i < fields; ++i) {
        // Find the field that has the least number of matching rules
        int min_valid = INT_MAX;
        int min_field = 0;
        int min_rule = 0;
        for (int f = 0; f < fields; ++f) {
            if (field_done[f]) continue;
            int valid_rules = 0;
            int mrule = -1;
            for (int r = 0; r < fields; ++r) {
                if (rule_field[r] < 0 && !invalid[f][r]) {
                    ++valid_rules;
                    mrule = r;
                }
            }
            if (valid_rules < min_valid) {
                min_valid = valid_rules;
                min_field = f;
                min_rule = mrule;
            }
        }
        // Assign one of the matching rules to the field
        assert(min_rule >= 0);
        rule_field[min_rule] = min_field;
        field_done[min_field] = true;
    }
    // Multiply my departure fields
    int64_t part2 = 1;
    for (int r = 0; r < fields; ++r) {
        const string& name = get<0>(rules[r]);
        if (name.find("departure") == 0) part2 *= my_ticket[rule_field[r]];
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
