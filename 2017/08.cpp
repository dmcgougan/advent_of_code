/*
 * Problem 8, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <source_location>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, unsigned line = source_location::current().line())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(line);
    }
}

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, start = 0;
    while ((p = s.find(sep, start)) != string::npos) {
        if (p > start) ret.emplace_back(s.substr(start, p - start));
        start = p + sep.size();
    }
    if (start < s.size()) ret.emplace_back(s.substr(start));
    return ret;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<tuple<string, string, int, string, string, int>> prg;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " ");
        CHECK(s.size() == 7);
        prg.emplace_back(std::move(s[0]),
                         std::move(s[1]),
                         atoi(s[2].c_str()),
                         std::move(s[4]),
                         std::move(s[5]),
                         atoi(s[6].c_str()));
    }
    CHECK(!prg.empty());

    // Solve both parts
    int part1 = 0;
    int part2 = 0;
    {
        auto get_cond = [](const string& cond) {
            static const unordered_map<string, bool (*)(int, int)> conds{
                {"<", [](int a, int b) { return a < b; }},
                {"<=", [](int a, int b) { return a <= b; }},
                {">", [](int a, int b) { return a > b; }},
                {">=", [](int a, int b) { return a >= b; }},
                {"==", [](int a, int b) { return a == b; }},
                {"!=", [](int a, int b) { return a != b; }},
            };
            auto it = conds.find(cond);
            CHECK(it != conds.end());
            return it->second;
        };
        unordered_map<string, int> regs;
        for (const auto& [reg, op, imm, cond_reg, cond, cond_val] : prg) {
            if (get_cond(cond)(regs[cond_reg], cond_val)) {
                int& reg_ref = regs[reg];
                if (op == "inc") {
                    reg_ref += imm;
                } else {
                    CHECK(op == "dec");
                    reg_ref -= imm;
                }
                part2 = max(part2, reg_ref);
            }
        }
        CHECK(!regs.empty());
        part1 = ranges::max(regs | views::values);
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
