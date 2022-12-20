/*
 * Problem 5, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
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
    struct instruction
    {
        ll count;
        ll from;  // zero-based index
        ll to;    // zero-based index
    };
    vector<deque<char>> initial_stacks;
    vector<deque<char>> stacks;
    vector<instruction> instructions;

    // Parse the input
    string line;
    while (getline(cin, line) && line.find("1") == string::npos) {
        if (initial_stacks.empty()) {
            ll count = (line.size() + 1) / 4;
            initial_stacks = vector<deque<char>>(count);
        }
        for (ll i = 1, j = 0; i < ll(line.size()); i += 4, ++j) {
            if (line[i] != ' ') {
                initial_stacks[j].push_back(line[i]);
            }
        }
    }
    assert(getline(cin, line) && line.size() == 0);
    while (getline(cin, line)) {
        auto words = split(line, " ");
        assert(words.size() > 5);
        instructions.push_back(
            instruction{stoll(words[1]), stoll(words[3]) - 1, stoll(words[5]) - 1});
    }

    // Part 1
    stacks = initial_stacks;
    for (instruction instr : instructions) {
        for (ll i = 0; i < instr.count; ++i) {
            assert(!stacks[instr.from].empty());
            stacks[instr.to].push_front(stacks[instr.from].front());
            stacks[instr.from].pop_front();
        }
    }
    cout << "Part 1: ";
    for (auto d : stacks) {
        assert(!d.empty());
        cout << d.front();
    }
    cout << endl;

    // Part 2
    stacks = initial_stacks;
    deque<char> tmp;
    for (instruction instr : instructions) {
        tmp.clear();
        for (ll i = 0; i < instr.count; ++i) {
            assert(!stacks[instr.from].empty());
            tmp.push_front(stacks[instr.from].front());
            stacks[instr.from].pop_front();
        }
        for (char c : tmp) {
            stacks[instr.to].push_front(c);
        }
    }
    cout << "Part 2: ";
    for (auto d : stacks) {
        assert(!d.empty());
        cout << d.front();
    }
    cout << endl;

    return 0;
}
