/*
 * Problem 19, Advent of Code 2023
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
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
    // Parse input data
    map<string, vector<tuple<int, int, string>>> workflows;
    string line;
    while (getline(cin, line) && !line.empty()) {
        size_t i;
        string name;
        string steps_str;
        for (i = 0; i < line.size(); ++i) {
            if (line[i] == '{') {
                name = line.substr(0, i);
                steps_str = line.substr(i + 1, line.size() - i - 2);
                break;
            }
        }
        assert(i < line.size());
        vector<string> steps = split(steps_str, ",");
        vector<tuple<int, int, string>> workflow;
        for (i = 0; i < steps.size(); ++i) {
            const string& s = steps[i];
            int ix;
            int op;
            string wf_name;
            if (i < steps.size() - 1) {
                vector<string> f = split(s, ":");
                assert(f.size() == 2);
                static const map<char, int> p2ix = {{'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};
                ix = p2ix.find(f[0][0])->second;
                stringstream ss(f[0].substr(2));
                ss >> op;
                if (f[0][1] == '<') op = -op;
                wf_name = f[1];
            } else {
                ix = -1;
                op = 0;
                wf_name = s;
            }
            workflow.push_back(make_tuple(ix, op, wf_name));
        }
        workflows.insert(make_pair(name, workflow));
    }
    vector<array<int, 4>> parts;
    while (getline(cin, line)) {
        vector<string> values = split(line.substr(1, line.size() - 2), ",");
        assert(values.size() == 4);
        parts.emplace_back();
        for (int i = 0; i < 4; ++i) {
            stringstream ss(values[i].substr(2));
            ss >> parts.back()[i];
        }
    }

    // Part 1
    ll part1 = 0;
    for (array<int, 4> part : parts) {
        string wname = "in";
        for (;;) {
            if (wname == "A") {
                // Accepted
                part1 += part[0] + part[1] + part[2] + part[3];
                break;
            } else if (wname == "R") {
                // Rejected
                break;
            }
            auto iter = workflows.find(wname);
            assert(iter != workflows.end());
            const auto& workflow = iter->second;
            for (auto [ix, op, wf_dst] : workflow) {
                if (ix < 0) {
                    // Last element
                    wname = wf_dst;
                    break;
                }
                if ((op < 0 && part[ix] < -op) || (op > 0 && part[ix] > op)) {
                    // Meets condition
                    wname = wf_dst;
                    break;
                }
            }
        }
    }

    // Part 2
    // Do BFS search of all possible outcomes and keep track of the
    // limits that the variables have to obey
    ll part2 = 0;
    struct state
    {
        state(string n) : wf_name(n) { }
        string wf_name;
        int wf_pos = 0;
        array<int, 8> limits = {1, 4000, 1, 4000, 1, 4000, 1, 4000};
    };
    deque<state> q;
    q.push_back(state("in"));
    while (!q.empty()) {
        state s = q.front();
        q.pop_front();
        if (s.wf_name == "A") {
            // Accepted - add the combinations
            ll comb = 1;
            for (int ix = 0; ix < 4; ++ix) {
                comb *= ll(max(s.limits[2 * ix + 1] - s.limits[2 * ix] + 1, 0));
            }
            part2 += comb;
            continue;
        } else if (s.wf_name == "R") {
            // Rejected
            continue;
        }
        auto iter = workflows.find(s.wf_name);
        assert(iter != workflows.end());
        assert(s.wf_pos < int(iter->second.size()));
        auto [ix, op, wf_dst] = iter->second[s.wf_pos];
        if (ix < 0) {
            // Last element - this just contains a new workflow to go to
            s.wf_name = wf_dst;
            s.wf_pos = 0;
            q.push_back(s);
            continue;
        }
        if (op < 0) {
            op = -op;
            // To meet the criteria the variable must be smaller than op
            // Add case for when we meet the criteria
            state meet(s);
            meet.limits[ix * 2 + 1] = min(meet.limits[ix * 2 + 1], op - 1);
            meet.wf_name = wf_dst;
            meet.wf_pos = 0;
            q.push_back(meet);
            // Add case for when we do not meet the criteria
            s.limits[ix * 2] = max(s.limits[ix * 2], op);
            s.wf_pos++;
            q.push_back(s);
        } else {
            // To meet the criteria the variable must be greater than op
            // Add case for when we meet the criteria
            state meet(s);
            meet.limits[ix * 2] = max(meet.limits[ix * 2], op + 1);
            meet.wf_name = wf_dst;
            meet.wf_pos = 0;
            q.push_back(meet);
            // Add case for when we do not meet the criteria
            s.limits[ix * 2 + 1] = min(s.limits[ix * 2 + 1], op);
            s.wf_pos++;
            q.push_back(s);
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
