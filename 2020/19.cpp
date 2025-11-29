/*
 * Problem 19, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <regex>
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
    struct rule_t
    {
        char letter = 0;
        vector<vector<int>> seq;
    };
    unordered_map<int, rule_t> rules;
    string line;
    while (getline(cin, line) && !line.empty()) {
        vector<string> s = split(line, ": ");
        assert(s.size() == 2);
        int rule_num = atoi(s[0].c_str());
        auto& rule = rules[rule_num];
        if (s[1][0] == '\"') {
            rule.letter = s[1][1];
            continue;
        }
        s = split(s[1], " | ");
        for (const string& rlist : s) {
            vector<int>& v = rule.seq.emplace_back();
            vector<string> rv = split(rlist, " ");
            for (const string& r : rv) {
                v.push_back(atoi(r.c_str()));
            }
        }
    }
    vector<string> messages;
    while (getline(cin, line)) {
        messages.emplace_back(move(line));
    }

    // Convert rules 0, 31 and 42 to regular expressions
    auto gen_re = [&](auto& self, int rule_num) -> string {
        auto iter = rules.find(rule_num);
        assert(iter != rules.end());
        const struct rule_t& rule = iter->second;
        if (rule.letter) return string(1, rule.letter);
        string s("(");
        for (const vector<int>& rule_seq : rule.seq) {
            if (s.size() > 1) s += '|';
            for (int rnum : rule_seq) {
                s += self(self, rnum);
            }
        }
        s += ')';
        return s;
    };
    regex re_0(gen_re(gen_re, 0));
    regex re_31(gen_re(gen_re, 31));
    regex re_42(gen_re(gen_re, 42));

    // Solve part 1
    int part1 = 0;
    for (const string& msg : messages) {
        part1 += regex_match(msg, re_0);
    }

    // Solve part 2
    int part2 = 0;
    for (const string& msg : messages) {
        // 0: 8 11
        // 8: 42 | 42 8
        // 11: 42 31 | 42 11 31

        // Check rule 0
        size_t i = 0;
        int num_42 = 0, num_31 = 0;
        smatch m;
        while (regex_search(msg.begin() + i, msg.end(), m, re_42) && m.position() == 0) {
            i += m.length();
            ++num_42;
        }
        while (regex_search(msg.begin() + i, msg.end(), m, re_31) && m.position() == 0) {
            i += m.length();
            ++num_31;
        }
        if (i == msg.size() && num_31 && num_42 > num_31) ++part2;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
