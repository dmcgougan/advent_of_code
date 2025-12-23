/*
 * Problem 16, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <source_location>
#include <vector>

using namespace std;
using vec4i = array<int, 4>;

[[noreturn]] static void check_failed(unsigned int line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, source_location loc = source_location::current())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(loc.line());
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<array<vec4i, 3>> samples;
    vector<vec4i> code;
    int max_opcode = 0;
    {
        string line;
        for (;;) {
            CHECK(getline(in, line));
            if (line.empty()) break;
            auto& s = samples.emplace_back();
            CHECK(sscanf(line.c_str(),
                         "Before: [%d, %d, %d, %d]",
                         &s[0][0],
                         &s[0][1],
                         &s[0][2],
                         &s[0][3]) == 4);
            CHECK(getline(in, line) && !line.empty());
            CHECK(sscanf(line.c_str(), "%d %d %d %d", &s[1][0], &s[1][1], &s[1][2], &s[1][3]) == 4);
            max_opcode = max(max_opcode, s[1][0]);
            CHECK(getline(in, line) && !line.empty());
            CHECK(sscanf(line.c_str(),
                         "After: [%d, %d, %d, %d]",
                         &s[2][0],
                         &s[2][1],
                         &s[2][2],
                         &s[2][3]) == 4);
            CHECK(getline(in, line) && line.empty());
        }
        CHECK(getline(in, line) && line.empty());
        while (getline(in, line)) {
            CHECK(!line.empty());
            auto& c = code.emplace_back();
            CHECK(sscanf(line.c_str(), "%d %d %d %d", &c[0], &c[1], &c[2], &c[3]) == 4);
        }
    }

    // Instruction functions
    static const vector<function<void(const vec4i&, vec4i&)>> ifuncs = {
        [](const vec4i& i, vec4i& r) { r[i[3]] = r[i[1]] + r[i[2]]; },     // addr
        [](const vec4i& i, vec4i& r) { r[i[3]] = r[i[1]] + i[2]; },        // addi
        [](const vec4i& i, vec4i& r) { r[i[3]] = r[i[1]] * r[i[2]]; },     // mulr
        [](const vec4i& i, vec4i& r) { r[i[3]] = r[i[1]] * i[2]; },        // muli
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] & r[i[2]]); },   // banr
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] & i[2]); },      // bani
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] | r[i[2]]); },   // borr
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] | i[2]); },      // bori
        [](const vec4i& i, vec4i& r) { r[i[3]] = r[i[1]]; },               // setr
        [](const vec4i& i, vec4i& r) { r[i[3]] = i[1]; },                  // seti
        [](const vec4i& i, vec4i& r) { r[i[3]] = (i[1] > r[i[2]]); },      // gtir
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] > i[2]); },      // gtri
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] > r[i[2]]); },   // gtrr
        [](const vec4i& i, vec4i& r) { r[i[3]] = (i[1] == r[i[2]]); },     // eqir
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] == i[2]); },     // eqri
        [](const vec4i& i, vec4i& r) { r[i[3]] = (r[i[1]] == r[i[2]]); },  // eqrr
    };

    // Solve part 1
    int part1 = 0;
    {
        for (const auto& [before, instr, after] : samples) {
            int count = 0;
            for (auto& ifunc : ifuncs) {
                vec4i reg = before;
                ifunc(instr, reg);
                if (reg == after) ++count;
            }
            if (count >= 3) ++part1;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Check the set of possible opcodes for each function
        int opcodes = max_opcode + 1;
        CHECK(opcodes == int(ifuncs.size()) && opcodes < 32);
        vector<uint32_t> possibles(ifuncs.size(), (1u << opcodes) - 1);
        for (const auto& [before, instr, after] : samples) {
            for (int f = 0; f < opcodes; ++f) {
                vec4i reg = before;
                ifuncs[f](instr, reg);
                if (reg != after) {
                    possibles[f] &= ~(1u << instr[0]);
                }
            }
        }
        // Deduce what opcode each function has
        int found = 0;
        vector<function<void(const vec4i&, vec4i&)>> opcode_func(opcodes);
        vector<bool> func_done(opcodes);
        while (found < opcodes) {
            // Find unassigned functions that have only one candidate opcode
            bool progress = false;
            for (int f = 0; f < opcodes; ++f) {
                if (func_done[f]) continue;
                int count = 0;
                int opcode = 0;
                for (int i = 0; i < opcodes; ++i) {
                    if (!opcode_func[i] && (possibles[f] & (1u << i))) {
                        opcode = i;
                        ++count;
                    }
                }
                if (count != 1) continue;
                // Found the opcode for this function
                opcode_func[opcode] = ifuncs[f];
                func_done[f] = true;
                ++found;
                progress = true;
            }
            CHECK(progress);
        }
        // Run the code
        vec4i reg{};
        for (const auto& instr : code) opcode_func[instr[0]](instr, reg);
        part2 = reg[0];
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
