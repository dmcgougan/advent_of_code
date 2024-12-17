/*
 * Problem 17, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static ll combo(int operand, ll a, ll b, ll c)
{
    if (operand < 4) return operand;
    switch (operand) {
    case 4:
        return a;
    case 5:
        return b;
    case 6:
        return c;
    default:
        assert(false);
    }
}

static vector<int> run_program(ll a,
                               ll b,
                               ll c,
                               const vector<int>& program,
                               bool break_at_branch = false)
{
    vector<int> output;
    size_t pc = 0;
    while (pc < program.size()) {
        ll op = program[pc + 1];
        ll cop = combo(op, a, b, c);
        switch (program[pc]) {
        case 0:
            a = (cop < 63 ? a / (1 << cop) : 0);
            break;
        case 1:
            b ^= op;
            break;
        case 2:
            b = cop & 7;
            break;
        case 3:
            if (break_at_branch) return output;
            if (a) pc = op - 2;
            break;
        case 4:
            b ^= c;
            break;
        case 5:
            output.push_back(cop & 7);
            break;
        case 6:
            b = (cop < 63 ? a / (1 << cop) : 0);
            break;
        case 7:
            c = (cop < 63 ? a / (1 << cop) : 0);
            break;
        default:
            assert(false);
        }
        pc += 2;
    }
    return output;
}

static string int_list(const vector<int>& list)
{
    string s;
    for (int v : list) {
        if (!s.empty()) s += ',';
        s += to_string(v);
    }
    return s;
}

// Resursively find value of A that outputs the program itself
static bool find_a(ll a, ll b, ll c, const vector<int>& program, int pc, ll& a_out)
{
    for (int i = 0; i < 8; i++) {
        vector<int> output = run_program((a << 3) | i, b, c, program, true);
        assert(output.size() == 1);
        if (output.back() == program[pc]) {
            if (pc == 0) {
                a_out = (a << 3) | i;
                return true;
            } else if (find_a((a << 3) | i, b, c, program, pc - 1, a_out)) {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    string line;
    getline(cin, line);
    ll a = strtoll(line.substr(12).c_str(), NULL, 10);
    getline(cin, line);
    ll b = strtoll(line.substr(12).c_str(), NULL, 10);
    getline(cin, line);
    ll c = strtoll(line.substr(12).c_str(), NULL, 10);
    getline(cin, line);
    getline(cin, line);
    stringstream ss(line.substr(9));
    vector<int> program;
    int opcode;
    while (ss >> opcode) {
        program.push_back(opcode);
        char c;
        ss >> c;
    }

    // Part 1
    cout << "Part 1: " << int_list(run_program(a, b, c, program)) << endl;

    // Part 2
    // This code assumes the program ends with a branch to the beginning until A is zero
    // and outputs data based on the 6 lowest bits of A at each iteration.
    assert(find_a(0, b, c, program, program.size() - 1, a));
    cout << "Part 2: " << a << endl;

    return 0;
}
