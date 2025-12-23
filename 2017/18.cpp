/*
 * Problem 18, Advent of Code 2017
 * Danjel McGougan
 */

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <source_location>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;

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
    vector<array<string, 3>> prg;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& [instr, p0, p1] = prg.emplace_back();
        CHECK(ss >> instr);
        CHECK(ss >> p0);
        ss >> p1;
        CHECK(instr == "snd" || instr == "rcv" || !p1.empty());
    }
    CHECK(!prg.empty());
    int psize = prg.size();

    // Common
    using regs_t = unordered_map<string, ll>;
    static const unordered_map<string, ll (*)(regs_t&, const string&, ll p1)> instr_func{
        {"set",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0] = p1;
             return 1ll;
         }},
        {"add",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0] += p1;
             return 1ll;
         }},
        {"mul",
         [](regs_t& regs, const string& p0, ll p1) {
             regs[p0] *= p1;
             return 1ll;
         }},
        {"mod",
         [](regs_t& regs, const string& p0, ll p1) {
             CHECK(p1);
             regs[p0] %= p1;
             return 1ll;
         }},
        {"jgz",
         [](regs_t& regs, const string& p0, ll p1) {
             if ((islower(p0[0]) ? regs[p0] : atoll(p0.c_str())) > 0) return p1;
             return 1ll;
         }},
    };

    // Solve part 1
    ll part1 = 0;
    {
        ll pc = 0;
        unordered_map<string, ll> regs;
        for (;;) {
            if (pc < 0 || pc >= psize) break;
            const auto& [instr, p0, p1] = prg[pc];
            if (instr == "rcv") {
                if (regs[p0]) {
                    part1 = regs["snd"];
                    break;
                }
                ++pc;
            } else if (instr == "snd") {
                regs["snd"] = (islower(p0[0]) ? regs[p0] : atoll(p0.c_str()));
                ++pc;
            } else {
                auto it = instr_func.find(instr);
                CHECK(it != instr_func.end());
                pc += it->second(regs, p0, islower(p1[0]) ? regs[p1] : atoll(p1.c_str()));
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        auto run = [&](regs_t& regs, int& pc, queue<ll>& snd_q, queue<ll>& rcv_q) {
            int snd_count = 0;
            for (;;) {
                if (pc < 0 || pc >= psize) return snd_count;
                const auto& [instr, p0, p1] = prg[pc];
                if (instr == "rcv") {
                    if (rcv_q.empty()) return snd_count;
                    regs[p0] = rcv_q.front();
                    rcv_q.pop();
                    ++pc;
                } else if (instr == "snd") {
                    snd_q.push(islower(p0[0]) ? regs[p0] : atoll(p0.c_str()));
                    ++pc;
                    ++snd_count;
                } else {
                    auto it = instr_func.find(instr);
                    CHECK(it != instr_func.end());
                    pc += it->second(regs, p0, islower(p1[0]) ? regs[p1] : atoll(p1.c_str()));
                }
            }
        };
        regs_t regs[2];
        int pc[2]{};
        queue<ll> q[2];
        regs[0]["p"] = 0;
        regs[1]["p"] = 1;
        for (;;) {
            int s0 = run(regs[0], pc[0], q[0], q[1]);
            int s1 = run(regs[1], pc[1], q[1], q[0]);
            part2 += s1;
            // Break if no progress is being made (either halted or waiting in rcv)
            if (!s0 && !s1) break;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
