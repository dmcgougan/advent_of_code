/*
 * Problem 23, Advent of Code 2017
 * Danjel McGougan
 */

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;
using regs_t = unordered_map<char, ll>;

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

static bool is_reg(char c) {
    return c >= 'a' && c <= 'h';
}

static ll value(regs_t& regs, const string& p)
{
    return is_reg(p[0]) ? regs[p[0]] : atoll(p.c_str());
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<array<string, 3>> prg;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& [instr, p0, p1] = prg.emplace_back();
        CHECK(ss >> instr);
        CHECK((ss >> p0) && p0.size() > 0 && (!is_reg(p0[0]) || p0.size() == 1));
        CHECK((ss >> p1) && p1.size() > 0 && (!is_reg(p1[0]) || p1.size() == 1));
    }
    int psize = prg.size();
    CHECK(psize);

    // Common
    static const unordered_map<string, int (*)(regs_t&, const string&, ll p1)> instr_func{
        {"set",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0[0]] = p1;
             return 1;
         }},
        {"sub",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0[0]] -= p1;
             return 1;
         }},
        {"mul",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0[0]] *= p1;
             return 1;
         }},
        {"jnz",
         [](regs_t& regs, const string& p0, ll p1) {
             if (value(regs, p0)) return static_cast<int>(p1);
             return 1;
         }},
    };

    // Solve part 1
    int part1 = 0;
    {
        int pc = 0;
        regs_t regs;
        while (pc >= 0 && pc < psize) {
            const auto& [instr, p0, p1] = prg[pc];
            auto it = instr_func.find(instr);
            CHECK(it != instr_func.end());
            pc += it->second(regs, p0, value(regs, p1));
            if (instr == "mul") ++part1;
        }
    }

    // Solve part 2
    int part2;
    {
        // Reverse engineered and optimized code
        auto code = []() {
            int h = 0;
            for (int b = 109900; b <= 109900 + 17000; b += 17) {
                if (b % 2 == 0) {
                    ++h;
                    continue;
                }
                for (int d = 3; d * d <= b; d += 2) {
                    if (b % d == 0) {
                        ++h;
                        break;
                    }
                }
            }
            return h;
        };
        part2 = code();
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
