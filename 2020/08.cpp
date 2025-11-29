/*
 * Problem 8, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static bool terminates(const vector<pair<string, int>>& code, int* acc_p = nullptr)
{
    int acc = 0;
    size_t pc = 0;
    vector<bool> visited(code.size(), false);
    while (pc < code.size() && !visited[pc]) {
        visited[pc] = true;
        const auto& [op, arg] = code[pc];
        if (op == "acc") {
            acc += arg;
        } else if (op == "jmp") {
            pc += arg - 1;
        }
        ++pc;
    }
    if (acc_p) *acc_p = acc;
    return pc == code.size();
}

int main()
{
    // Parse input
    string line;
    vector<pair<string, int>> code;
    while (getline(cin, line)) {
        assert(line.size() >= 6 && line[3] == ' ');
        code.push_back({line.substr(0, 3), atoi(line.substr(4).c_str())});
    }

    // Solve part 1
    int part1;
    assert(!terminates(code, &part1));

    // Solve part 2
    int part2;
    for (size_t i = 0; i < code.size() || (assert(false), 0); ++i) {
        auto& [op, arg] = code[i];
        if (op == "jmp") {
            op = "nop";
            if (terminates(code, &part2)) break;
            op = "jmp";
        } else if (op == "nop") {
            op = "jmp";
            if (terminates(code, &part2)) break;
            op = "nop";
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
