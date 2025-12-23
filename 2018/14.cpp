/*
 * Problem 14, Advent of Code 2018
 * Danjel McGougan
 */

#include <fstream>
#include <iostream>
#include <print>
#include <source_location>

using namespace std;

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
    int count;
    CHECK(in >> count);

    // Common
    auto step = [&](string& r, int& e0, int& e1) -> void {
        int sum = (r[e0] - '0') + (r[e1] - '0');
        if (sum < 10) {
            r += sum + '0';
        } else {
            r += sum / 10 + '0';
            r += sum % 10 + '0';
        }
        e0 = (e0 + r[e0] - '0' + 1) % r.size();
        e1 = (e1 + r[e1] - '0' + 1) % r.size();
    };

    // Solve part 1
    string part1;
    {
        string r = "37";
        int e0 = 0;
        int e1 = 1;
        while (int(r.size()) < count + 10) step(r, e0, e1);
        part1 = r.substr(count, 10);
    }

    // Solve part 2
    int part2;
    {
        string r = "37";
        int e0 = 0;
        int e1 = 1;
        string f = to_string(count);
        for (;;) {
            step(r, e0, e1);
            if (r.size() >= f.size() + 1 && r.compare(r.size() - f.size() - 1, f.size(), f) == 0) {
                part2 = r.size() - f.size() - 1;
                break;
            }
            if (r.size() >= f.size() && r.compare(r.size() - f.size(), f.size(), f) == 0) {
                part2 = r.size() - f.size();
                break;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
