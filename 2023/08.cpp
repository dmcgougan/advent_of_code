/*
 * Problem 8, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>

using namespace std;
using ll = int64_t;

int main()
{
    // Parse input data
    map<string, pair<string, string>> nodes;
    string instr;
    getline(cin, instr);
    int instrcount = instr.size();
    string line;
    getline(cin, line);
    while (getline(cin, line)) {
        stringstream ss(line);
        string from, left, right, s;
        ss >> from;
        ss >> s;
        ss >> left;
        ss >> right;
        left = left.substr(1, 3);
        right = right.substr(0, 3);
        nodes[from] = make_pair(left, right);
    }

    // Part 1
    ll part1 = 0;
    auto loc = nodes.find("AAA");
    int i = 0;
    for (;;) {
        char c = instr[i];
        if (c == 'L') {
            loc = nodes.find(loc->second.first);
        } else {
            loc = nodes.find(loc->second.second);
        }
        ++part1;
        if (loc->first == "ZZZ") break;
        i = (i + 1) % instrcount;
    }

    // Part 2
    ll part2 = 1;
    for (auto bloc = nodes.begin(); bloc != nodes.end(); ++bloc) {
        if (bloc->first[2] != 'A') continue;
        auto loc = bloc;
        i = 0;
        ll period = 0;
        for (;;) {
            char c = instr[i];
            if (c == 'L') {
                loc = nodes.find(loc->second.first);
            } else {
                loc = nodes.find(loc->second.second);
            }
            ++period;
            if (loc->first[2] == 'Z') break;
            i = (i + 1) % instrcount;
        }
        // Find Least Common Divisor of all periods
        part2 = lcm(part2, period);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
