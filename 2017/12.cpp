/*
 * Problem 12, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
#include <source_location>
#include <sstream>
#include <string>
#include <utility>
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

// Disjoint Set Union
// https://cp-algorithms.com/data_structures/disjoint_set_union.html
class dsu_t
{
public:
    explicit dsu_t(int n) : parent(n), sz(n, 1), sets(n) { ranges::iota(parent, 0); }

    // Get the representative (root) of the set that x belongs to
    int find(int x)
    {
        if (x == parent[x]) return x;
        // Flatten the tree while searching
        return (parent[x] = find(parent[x]));
    }

    // Merge sets a and b
    bool merge(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        --sets;
        return true;
    }

    // Get the size of the set that x belongs to
    int size(int x) { return sz[find(x)]; }

    // Get the number of disjoint sets
    int count() const { return sets; }

private:
    vector<int> parent;
    vector<int> sz;
    int sets;
};

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, p0 = 0;
    while ((p = s.find(sep, p0)) != string::npos) {
        if (p > p0) ret.emplace_back(s.substr(p0, p - p0));
        p0 = p + sep.size();
    }
    if (p0 < s.size()) ret.emplace_back(s.substr(p0));
    return ret;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<pair<int, vector<int>>> pipes;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " <-> ");
        CHECK(s.size() == 2);
        auto& [from, v] = pipes.emplace_back();
        from = atoi(s[0].c_str());
        stringstream ss(s[1]);
        for (int to; ss >> to;) {
            v.push_back(to);
            ss.ignore(1);
        }
    }
    int psize = pipes.size();

    // Solve parts 1 and 2
    int part1 = 0;
    int part2 = 0;
    {
        dsu_t dsu(psize);
        for (const auto& [from, v] : pipes) {
            CHECK(from >= 0 && from < psize);
            for (auto to : v) {
                CHECK(to >= 0 && to < psize);
                dsu.merge(from, to);
            }
        }
        part1 = dsu.size(0);
        part2 = dsu.count();
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
