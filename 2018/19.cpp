/*
 * Problem 19, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using vec4i = array<int, 4>;
using vec6i = array<int64_t, 6>;

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

    // Parse input
    int ip_reg;
    vector<pair<string, vec4i>> prg;
    {
        string line;
        CHECK(getline(in, line) && line.size() > 4);
        ip_reg = atoi(line.c_str() + 4);
        while (getline(in, line)) {
            stringstream ss(std::move(line));
            auto& [name, param] = prg.emplace_back();
            CHECK(ss >> name);
            CHECK(ss >> param[1]);
            CHECK(ss >> param[2]);
            CHECK(ss >> param[3]);
        }
    }
    int psize = prg.size();

    // Instruction functions
    static const unordered_map<string, void (*)(const vec4i&, vec6i&)> ifuncs = {
        {"addr", +[](const vec4i& i, vec6i& r) { r[i[3]] = r[i[1]] + r[i[2]]; }},
        {"addi", +[](const vec4i& i, vec6i& r) { r[i[3]] = r[i[1]] + i[2]; }},
        {"mulr", +[](const vec4i& i, vec6i& r) { r[i[3]] = r[i[1]] * r[i[2]]; }},
        {"muli", +[](const vec4i& i, vec6i& r) { r[i[3]] = r[i[1]] * i[2]; }},
        {"banr", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] & r[i[2]]); }},
        {"bani", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] & i[2]); }},
        {"borr", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] | r[i[2]]); }},
        {"bori", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] | i[2]); }},
        {"setr", +[](const vec4i& i, vec6i& r) { r[i[3]] = r[i[1]]; }},
        {"seti", +[](const vec4i& i, vec6i& r) { r[i[3]] = i[1]; }},
        {"gtir", +[](const vec4i& i, vec6i& r) { r[i[3]] = (i[1] > r[i[2]]); }},
        {"gtri", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] > i[2]); }},
        {"gtrr", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] > r[i[2]]); }},
        {"eqir", +[](const vec4i& i, vec6i& r) { r[i[3]] = (i[1] == r[i[2]]); }},
        {"eqri", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] == i[2]); }},
        {"eqrr", +[](const vec4i& i, vec6i& r) { r[i[3]] = (r[i[1]] == r[i[2]]); }},
    };

    // Solve part 1
    int part1 = 0;
    {
        vec6i reg{};
        while (reg[ip_reg] >= 0 && reg[ip_reg] < psize) {
            const auto& [name, param] = prg[reg[ip_reg]];
            auto it = ifuncs.find(name);
            CHECK(it != ifuncs.end());
            it->second(param, reg);
            ++reg[ip_reg];
        }
        part1 = reg[0];
    }

    // Solve part 2
    int64_t part2 = 0;
    {
        vec6i reg{1};
        while (reg[ip_reg] >= 0 && reg[ip_reg] < psize) {
            const auto& [name, param] = prg[reg[ip_reg]];
            auto it = ifuncs.find(name);
            CHECK(it != ifuncs.end());
            it->second(param, reg);
            ++reg[ip_reg];
            if (reg[ip_reg] == 1) {
                // Register 5 has been setup for the slow loop; break here
                break;
            }
        }
        // Find sum of divisors of the value in register 5
        int64_t N = reg[5];
        for (int64_t d = 1; d * d <= N; d++) {
            if (N % d == 0) {
                part2 += d;
                if (d * d != N) part2 += N / d;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
