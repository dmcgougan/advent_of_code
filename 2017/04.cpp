/*
 * Problem 4, Advent of Code 2017
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>
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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<vector<string>> phrases;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& v = phrases.emplace_back();
        string word;
        while (ss >> word) v.emplace_back(std::move(word));
    }
    CHECK(!phrases.empty());

    // Common; check pairs of items for any match
    auto any_pair = [](const auto& v, const auto& pred) {
        for (size_t i = 0; i + 1 < v.size(); ++i) {
            for (size_t j = i + 1; j < v.size(); ++j) {
                if (pred(v[i], v[j])) return true;
            }
        }
        return false;
    };

    // Solve part 1
    int part1 = 0;
    {
        for (const vector<string>& p : phrases) {
            if (!any_pair(p, equal_to<>{})) ++part1;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        auto is_anagram = [](string_view a, string_view b) {
            if (a.size() != b.size()) return false;
            array<int, UCHAR_MAX + 1> cnt{};
            for (size_t i = 0; i < a.size(); ++i) {
                ++cnt[static_cast<unsigned char>(a[i])];
                --cnt[static_cast<unsigned char>(b[i])];
            }
            for (int x : cnt) {
                if (x) return false;
            }
            return true;
        };
        for (const vector<string>& p : phrases) {
            if (!any_pair(p, is_anagram)) ++part2;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
