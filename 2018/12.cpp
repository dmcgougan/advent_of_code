/*
 * Problem 12, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
#include <set>
#include <source_location>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = long long;

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
    set<ll> initial_state;
    vector<bool> new_plant(32);
    {
        string line;
        CHECK(getline(in, line) && line.size() > 15);
        line = line.substr(15);
        for (int p = 0; p < int(line.size()); ++p) {
            if (line[p] == '#') initial_state.insert(p);
        }
        CHECK(getline(in, line) && line.empty());
        while (getline(in, line)) {
            CHECK(line.size() == 10);
            if (line[9] != '#') continue;
            unsigned pattern = 0;
            for (int i = 0, b = (1 << 4); i < 5; ++i, b >>= 1) {
                pattern |= (line[i] == '#' ? b : 0);
            }
            CHECK(pattern < new_plant.size());
            new_plant[pattern] = true;
        }
    }

    // Common
    auto next = [&](set<ll>& state) -> void {
        if (state.empty()) return;
        set<ll> old = state;
        state.clear();
        ll min = *old.begin();
        ll max = *old.rbegin();
        unsigned pattern = 0;
        for (ll p = min - 2; p <= max + 2; ++p) {
            pattern = ((pattern << 1) | old.count(p + 2)) & 0x1f;
            if (new_plant[pattern]) state.insert(p);
        }
    };

    // Solve part 1
    ll part1;
    {
        set<ll> state = initial_state;
        for (int i = 0; i < 20; ++i) {
            next(state);
        }
        part1 = accumulate(state.begin(), state.end(), 0);
    }

    // Solve part 2
    ll part2;
    {
        // Hash of plant distribution (disregarding constant shift)
        auto hash = [](const set<ll>& state) -> ll {
            ll min = *state.begin();
            ll h = 0;
            for (ll p : state) {
                h ^= ll(p - min) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        };
        const ll N = 50000000000;
        set<ll> state = initial_state;
        unordered_map<ll, array<ll, 2>> seen;
        seen[hash(state)] = {0, *state.begin()};
        ll cadd = 0;
        for (ll i = 1; i <= N; ++i) {
            next(state);
            if (cadd) continue;
            ll h = hash(state);
            auto iter = seen.find(h);
            if (iter != seen.end()) {
                // We have seen this state before; we can jump ahead.
                // It turns out the pattern stays constant when reaching this state
                // at least for my input. This code also handles the situation when
                // the pattern is cyclic with a period greater than one.
                ll steps = i - iter->second[0];
                ll diff = *state.begin() - iter->second[1];
                ll jumps = (N - i) / steps;
                cadd = jumps * diff;
                i += jumps * steps;
                continue;
            }
            seen[h] = {i, *state.begin()};
        }
        part2 = accumulate(state.begin(), state.end(), 0) + cadd * state.size();
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
