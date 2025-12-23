/*
 * Problem 25, Advent of Code 2017
 * Danjel McGougan
 */

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <tuple>
#include <unordered_map>

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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    using action_t = tuple<bool, bool, char>;
    unordered_map<char, array<action_t, 2>> prg;
    char start_state;
    int steps;
    {
        string line;
        size_t pos;
        CHECK(getline(in, line) && line.size() >= 2);
        start_state = line[line.size() - 2];
        CHECK(getline(in, line) && !line.empty());
        CHECK((pos = line.find_first_of("0123456789")) != string::npos);
        steps = atoi(line.c_str() + pos);
        CHECK(getline(in, line) && line.empty());
        for (;;) {
            CHECK(getline(in, line) && !line.empty());
            CHECK(line.size() >= 2);
            char state = line[line.size() - 2];
            auto& actions = prg[state];
            for (int i = 0; i < 2; ++i) {
                auto& [value, move, next] = actions[i];
                CHECK(getline(in, line) && !line.empty());
                CHECK(getline(in, line) && line.size() >= 2);
                value = (line[line.size() - 2] == '1');
                CHECK(getline(in, line) && line.size() >= 5);
                move = (line[line.size() - 5] == 'i');
                CHECK(getline(in, line) && line.size() >= 2);
                next = line[line.size() - 2];
            }
            if (!getline(in, line)) break;
            CHECK(line.empty());
        }
    }

    // Solve part 1
    int part1 = 0;
    {
        unordered_map<int, bool> tape;
        int tape_pos = 0;
        char state = start_state;
        for (int i = 0; i < steps; ++i) {
            auto it = prg.find(state);
            CHECK(it != prg.end());
            const auto& actions = it->second;
            bool& t = tape[tape_pos];
            const auto& [value, move, next] = actions[t];
            if (!t && value) ++part1;
            if (t && !value) --part1;
            t = value;
            tape_pos += move ? 1 : -1;
            state = next;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: <click>");

    return 0;
}
