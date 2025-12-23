/*
 * Problem 20, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
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

template <class T, size_t N>
struct array_hash_t
{
    size_t operator()(const array<T, N>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& x : a) {
            h ^= hash<T>{}(x) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<array<ll, 9>> particles;
    ll max_abs_value = 0;
    for (string line; getline(in, line);) {
        auto& p = particles.emplace_back();
        CHECK(sscanf(line.c_str(),
                     "p=<%lld,%lld,%lld>, v=<%lld,%lld,%lld>, a=<%lld,%lld,%lld>",
                     &p[0],
                     &p[1],
                     &p[2],
                     &p[3],
                     &p[4],
                     &p[5],
                     &p[6],
                     &p[7],
                     &p[8]) == 9);
        for (ll v : p) max_abs_value = max(max_abs_value, abs(v));
    }

    // Solve part 1
    ll part1 = 0;
    {
        auto less = [](const array<ll, 9>& p0, const array<ll, 9>& p1) {
            ll acc0 = abs(p0[6]) + abs(p0[7]) + abs(p0[8]);
            ll acc1 = abs(p1[6]) + abs(p1[7]) + abs(p1[8]);
            if (acc0 < acc1) return true;
            if (acc0 > acc1) return false;
            ll vel0 = abs(p0[3]) + abs(p0[4]) + abs(p0[5]);
            ll vel1 = abs(p1[3]) + abs(p1[4]) + abs(p1[5]);
            if (vel0 < vel1) return true;
            if (vel0 > vel1) return false;
            ll pos0 = abs(p0[0]) + abs(p0[1]) + abs(p0[2]);
            ll pos1 = abs(p1[0]) + abs(p1[1]) + abs(p1[2]);
            if (pos0 < pos1) return true;
            return false;
        };
        part1 = distance(particles.begin(), ranges::min_element(particles, less));
    }

    // Solve part 2
    int part2 = 0;
    {
        unordered_map<array<ll, 3>, int, array_hash_t<ll, 3>> seen;
        vector<size_t> remove;
        int steps_since_collision = 0;
        part2 = particles.size();
        while (steps_since_collision < max_abs_value) {
            ++steps_since_collision;
            for (size_t i = 0; i < particles.size(); ++i) {
                auto& [px, py, pz, vx, vy, vz, ax, ay, az] = particles[i];
                vx += ax;
                vy += ay;
                vz += az;
                px += vx;
                py += vy;
                pz += vz;
                auto it = seen.find({px, py, pz});
                if (it == seen.end()) {
                    seen[{px, py, pz}] = i;
                } else {
                    if (it->second >= 0) remove.push_back(it->second);
                    it->second = -1;
                    remove.push_back(i);
                }
            }
            seen.clear();
            if (!remove.empty()) {
                steps_since_collision = 0;
                part2 -= remove.size();
                ranges::sort(remove, greater());
                for (size_t i : remove) {
                    particles.erase(particles.begin() + i);
                }
                remove.clear();
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
