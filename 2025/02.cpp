/*
 * Problem 2, Advent of Code 2025
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

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
    assert(in);

    // Parse input
    vector<array<int64_t, 2>> ids;
    {
        string line;
        assert(getline(in, line));
        for (const string& range : split(line, ",")) {
            vector<string> p = split(range, "-");
            assert(p.size() == 2);
            ids.push_back({int64_t(std::stoll(p[0])), int64_t(std::stoll(p[1]))});
        }
        assert(!ids.empty());
    }

    // Solve part 1
    int64_t part1 = 0;
    {
        auto valid = [](int64_t id) -> bool {
            string s = std::to_string(id);
            int len = s.size();
            if (len % 2) return true;
            for (int i = 0; i < len / 2; ++i) {
                if (s[i] != s[i + len / 2]) return true;
            }
            return false;
        };
        for (auto [from, to] : ids) {
            for (int64_t id = from; id <= to; ++id) {
                if (!valid(id)) part1 += id;
            }
        }
    }

    // Solve part 2
    int64_t part2 = 0;
    {
        auto valid = [](int64_t id) -> bool {
            string s = std::to_string(id);
            int len = s.size();
            for (int p = 2; len / p >= 1; ++p) {
                // Try to divide the string into p equal parts
                if (len % p) continue;
                int plen = len / p;
                bool repeat = true;
                for (int i = 0; i < plen && repeat; ++i) {
                    // Check if something does not repeat
                    for (int j = 1; j < p; ++j) {
                        if (s[i] != s[i + j * plen]) {
                            repeat = false;
                            break;
                        }
                    }
                }
                if (repeat) return false;
            }
            return true;
        };
        for (auto [from, to] : ids) {
            for (int64_t id = from; id <= to; ++id) {
                if (!valid(id)) part2 += id;
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
