/*
 * Problem 16, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
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
    vector<tuple<char, int, int>> moves;
    {
        string line;
        CHECK(getline(in, line));
        vector<string> s = split(line, ",");
        for (const string& move : s) {
            CHECK(move.size() >= 2);
            if (move[0] == 's') {
                moves.emplace_back('s', atoi(move.c_str() + 1), 0);
            } else if (move[0] == 'x') {
                vector<string> xp = split(move, "/");
                CHECK(xp.size() == 2 && xp[0].size() > 0);
                moves.emplace_back('x', atoi(xp[0].c_str() + 1), atoi(xp[1].c_str()));
            } else {
                CHECK(move[0] == 'p' && move.size() == 4);
                moves.emplace_back('p', move[1] - 'a', move[3] - 'a');
            }
        }
    }
    CHECK(!moves.empty());

    // Common
    auto dance = [&](array<int, 16>& line, array<int, 16>& rline, int& p) {
        for (auto [m, p0, p1] : moves) {
            switch (m) {
            case 's':
                p = (p - p0) & 15;
                break;
            case 'x': {
                int i0 = (p0 + p) & 15;
                int i1 = (p1 + p) & 15;
                swap(line[i0], line[i1]);
                swap(rline[line[i0]], rline[line[i1]]);
                break;
            }
            case 'p':
                swap(rline[p0], rline[p1]);
                swap(line[rline[p0]], line[rline[p1]]);
                break;
            default:
                break;
            }
        }
    };
    auto get_line = [&](const array<int, 16>& line, int p, bool add_p = false) {
        string s(16 + add_p, 0);
        for (int i = 0; i < 16; ++i) {
            s[i] = line[(p + i) & 15] + 'a';
        }
        if (add_p) s[16] = p + 'a';
        return s;
    };

    // Solve part 1
    string part1;
    {
        array<int, 16> line;
        array<int, 16> rline;
        ranges::iota(line, 0);
        ranges::iota(rline, 0);
        int p = 0;
        dance(line, rline, p);
        part1 = get_line(line, p);
    }

    // Solve part 2
    string part2;
    {
        array<int, 16> line;
        array<int, 16> rline;
        ranges::iota(line, 0);
        ranges::iota(rline, 0);
        int p = 0;

        // Keep track of seen states and at which step they appeared
        unordered_map<string, int> seen;
        seen.emplace(get_line(line, p, true), 0);

        // Do one billion steps
        constexpr int N = 1'000'000'000;
        int steps = 0;
        while (steps < N) {
            dance(line, rline, p);
            ++steps;

            // Check if we've seen this state before
            string state = get_line(line, p, true);
            auto it = seen.find(state);
            if (it == seen.end()) {
                // No, store it
                seen.emplace(std::move(state), steps);
            } else {
                // Found a previous seen state; jump forward.
                // We can jump forward a multiple of the cycle period because
                // the state will return back to where it is right now.
                int period = steps - it->second;
                steps += (N - steps) / period * period;
            }
        }
        part2 = get_line(line, p);
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
