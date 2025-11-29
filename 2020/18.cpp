/*
 * Problem 18, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

static int64_t parse_p1(const string& s)
{
    size_t pos = 0;
    auto rec = [&](auto& self) -> int64_t {
        bool op_add = true;
        int64_t result = 0;
        for (; pos < s.size(); ++pos) {
            char ch = s[pos];
            switch (ch) {
            case ' ':
                break;
            case '+':
                op_add = true;
                break;
            case '*':
                op_add = false;
                break;
            case '(': {
                ++pos;
                int64_t value = self(self);
                op_add ? result += value : result *= value;
                break;
            }
            case ')':
                return result;
            default:
                assert(ch >= '0' && ch <= '9');
                op_add ? result += ch - '0' : result *= ch - '0';
                break;
            }
        }
        return result;
    };
    return rec(rec);
}

static int64_t parse_p2(const string& s)
{
    // Recursive descent parser
    // add_operand := number | "(" expr ")"
    // mul_operand := add_operand { "+" add_operand }
    // expr        := mul_operand { "*" mul_operand }
    size_t pos = 0;
    auto add_operand = [&](auto& expr) -> int64_t {
        while (pos < s.size() && isspace(s[pos])) ++pos;
        if (s[pos] == '(') {
            ++pos;
            int64_t val = expr(expr);
            while (pos < s.size() && isspace(s[pos])) ++pos;
            if (s[pos] == ')') ++pos;
            return val;
        }
        assert(s[pos] >= '0' && s[pos] <= '9');
        return s[pos++] - '0';
    };
    auto mul_operand = [&](auto& expr) -> int64_t {
        int64_t val = add_operand(expr);
        for (;;) {
            while (pos < s.size() && isspace(s[pos])) ++pos;
            if (pos < s.size() && s[pos] == '+') {
                ++pos;
                val += add_operand(expr);
            } else {
                break;
            }
        }
        return val;
    };
    auto expr = [&](auto& self) -> int64_t {
        int64_t val = mul_operand(self);
        for (;;) {
            while (pos < s.size() && isspace(s[pos])) ++pos;
            if (pos < s.size() && s[pos] == '*') {
                ++pos;
                val *= mul_operand(self);
            } else {
                break;
            }
        }
        return val;
    };
    return expr(expr);
}

int main()
{
    // Parse input and solve
    string line;
    int64_t part1 = 0;
    int64_t part2 = 0;
    while (getline(cin, line)) {
        part1 += parse_p1(line);
        part2 += parse_p2(line);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
