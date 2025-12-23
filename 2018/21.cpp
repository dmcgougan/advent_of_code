/*
 * Problem 21, Advent of Code 2018
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
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;
using vec3i = array<int, 3>;
using vec6i = array<ll, 6>;

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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Instruction functions
    using op_t = void (*)(const vec3i&, vec6i&);
    static const unordered_map<string, op_t> ifuncs = {
        {"addr", +[](const vec3i& i, vec6i& r) { r[i[2]] = r[i[0]] + r[i[1]]; }},
        {"addi", +[](const vec3i& i, vec6i& r) { r[i[2]] = r[i[0]] + i[1]; }},
        {"mulr", +[](const vec3i& i, vec6i& r) { r[i[2]] = r[i[0]] * r[i[1]]; }},
        {"muli", +[](const vec3i& i, vec6i& r) { r[i[2]] = r[i[0]] * i[1]; }},
        {"banr", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] & r[i[1]]); }},
        {"bani", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] & i[1]); }},
        {"borr", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] | r[i[1]]); }},
        {"bori", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] | i[1]); }},
        {"setr", +[](const vec3i& i, vec6i& r) { r[i[2]] = r[i[0]]; }},
        {"seti", +[](const vec3i& i, vec6i& r) { r[i[2]] = i[0]; }},
        {"gtir", +[](const vec3i& i, vec6i& r) { r[i[2]] = (i[0] > r[i[1]]); }},
        {"gtri", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] > i[1]); }},
        {"gtrr", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] > r[i[1]]); }},
        {"eqir", +[](const vec3i& i, vec6i& r) { r[i[2]] = (i[0] == r[i[1]]); }},
        {"eqri", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] == i[1]); }},
        {"eqrr", +[](const vec3i& i, vec6i& r) { r[i[2]] = (r[i[0]] == r[i[1]]); }},
    };

    // Parse input
    int ip_reg;
    vector<pair<op_t, vec3i>> prg;
    {
        string line;
        CHECK(getline(in, line) && line.size() > 4);
        ip_reg = atoi(line.c_str() + 4);
        while (getline(in, line)) {
            stringstream ss(line);
            auto& [func, param] = prg.emplace_back();
            string name;
            CHECK(ss >> name);
            auto it = ifuncs.find(name);
            CHECK(it != ifuncs.end());
            func = it->second;
            CHECK(ss >> param[0]);
            CHECK(ss >> param[1]);
            CHECK(ss >> param[2]);
        }
    }
    int psize = prg.size();

    // Solve part 1
    ll part1 = 0;
    {
        vec6i regs{};
        auto& ip = regs[ip_reg];
        while (ip != 28) {
            CHECK(ip >= 0 && ip < psize);
            const auto& [func, param] = prg[ip];
            func(param, regs);
            ++ip;
        }
        part1 = regs[5];
    }

    // Solve part 2
    ll part2 = 0;
    {
        unordered_set<ll> seen;
        vec6i regs{};
        auto& ip = regs[ip_reg];
        for (;;) {
            CHECK(ip >= 0 && ip < psize);
            const auto& [func, param] = prg[ip];
            func(param, regs);
            ++ip;
            if (ip == 28) {
                if (seen.contains(regs[5])) break;
                seen.insert(regs[5]);
                part2 = regs[5];
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
