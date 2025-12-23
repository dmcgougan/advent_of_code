/*
 * Problem 23, Advent of Code 2018
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <source_location>
#include <string>
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
    vector<array<ll, 4>> data;
    for (string line; getline(in, line);) {
        auto& d = data.emplace_back();
        CHECK(sscanf(line.c_str(), "pos=<%lld,%lld,%lld>, r=%lld", &d[1], &d[2], &d[3], &d[0]) ==
              4);
    }
    CHECK(!data.empty());

    // Common
    sort(data.begin(), data.end(), greater<array<ll, 4>>());

    // Solve part 1
    ll part1 = 0;
    {
        // Get the bot with the largest radius
        auto [r, x0, y0, z0] = data.front();
        for (const auto& [_, x1, y1, z1] : data) {
            // Check if the current bot is in range
            if (abs(x1 - x0) + abs(y1 - y0) + abs(z1 - z0) <= r) ++part1;
        }
    }

    // Solve part 2
    //
    // Use a transformation of the coordinates:
    //     u =  x + y + z
    //     v =  x + y - z
    //     w =  x - y + z
    //     s = -x + y + z
    //
    // Inverse:
    //     x = (u + v + w - s) / 4
    //     y = (u + v - w + s) / 4
    //     z = (u - v + w + s) / 4
    //
    // (u, v, w, s) must all have the same parity (all even or all odd) for the
    // inverse to yield integer (x, y, z).
    //
    // This transformation has the property that for a nanobot with
    // 4d coordinates (u_n, v_n, w_n, s_n) radius r_n, you can check
    // if a point with 4d coordinates (u_0, v_0, w_0, s_0) lies in
    // range of that nanobot with these range checks:
    //     u_n - r_n <= u_0 <= u_n + r_n
    //     v_n - r_n <= v_0 <= v_n + r_n
    //     w_n - r_n <= w_0 <= w_n + r_n
    //     s_n - r_n <= s_0 <= s_n + r_n
    // If these four conditions are true then the corresponding 3d point
    // is in range of the nanobot.
    //
    // It is like the bots are axis-aligned 4D hypercubes in (u, v, w, s) space;
    // to check if a point is in range you just check if it lies inside that cube.
    //
    // Manhattan distance to origin in 3d space can be calculated in 4d space as:
    // |x|+|y|+|z| = max(|u|,|v|,|w|,|s|)
    //
    // Strategy:
    //   * Maintain a set of axis-aligned search cubes in 4d space;
    //     one range per dimension, 8 integers:
    //       u_min, u_max, v_min, v_max, w_min, w_max, s_min, s_max
    //   * Before adding a cube to the set calculate:
    //       (1) How many bot cubes overlap with the search cube.
    //           This is an upper bound for how many bots that can cover a specific
    //           point in the cube. If a bot "covers" a point it means the point is
    //           in range of that bot. Call this function:
    //             bots_ub(C)
    //       (2) A lower bound on the 3d manhattan distance to origin for a specific
    //           point in the search cube C. We can choose this function:
    //             dist_lb(C) = max(min(|u_min|, |u_max|, u_min <= 0 && u_max >= 0 ? 0 : Inf),
    //                              min(|v_min|, |v_max|, v_min <= 0 && v_max >= 0 ? 0 : Inf),
    //                              min(|w_min|, |w_max|, w_min <= 0 && w_max >= 0 ? 0 : Inf),
    //                              min(|s_min|, |s_max|, s_min <= 0 && s_max >= 0 ? 0 : Inf))
    //       (3) Max side length of the search cube C:
    //             max_side(C)
    //   * Add a search cube that covers all bot cubes to the set.
    //   * Extract the search cube C from the set that has:
    //       (1) Highest bots_ub(C); if more than one
    //       (2) Lowest dist_lb(C); if more than one
    //       (3) Lowest max_side(C)
    //   * If the cube is larger than a single point:
    //         Split the cube along its largest axis into two cubes and add them to the set
    //     Otherwise:
    //         Check if the point parity matches an integer 3d point; if not discard.
    //         Calculate exact number of bots that covers the point and the 3d manhattan
    //         distance to the origin. Update the best seen (max coverage, min distance).
    //   * Continue extracting cubes from the set until it is empty.
    //
    ll part2 = 0;
    {
        vector<array<ll, 5>> bot_cubes;
        array<array<ll, 2>, 4> bounds{{{LLONG_MAX, LLONG_MIN},
                                       {LLONG_MAX, LLONG_MIN},
                                       {LLONG_MAX, LLONG_MIN},
                                       {LLONG_MAX, LLONG_MIN}}};
        for (const auto& [r, x, y, z] : data) {
            auto& cube = bot_cubes.emplace_back();
            cube[0] = x + y + z;
            cube[1] = x + y - z;
            cube[2] = x - y + z;
            cube[3] = -x + y + z;
            cube[4] = r;
            for (int i = 0; i < 4; ++i) {
                bounds[i][0] = min(bounds[i][0], cube[i] - r);
                bounds[i][1] = max(bounds[i][1], cube[i] + r);
            }
        }

        struct search_cube_t
        {
            static int bots_ub_f(const array<array<ll, 2>, 4>& rng, const vector<array<ll, 5>>& bc)
            {
                int count = 0;
                for (const auto& cube : bc) {
                    bool overlap = true;
                    for (int i = 0; i < 4; ++i) {
                        if (rng[i][1] < cube[i] - cube[4] || rng[i][0] > cube[i] + cube[4]) {
                            overlap = false;
                            break;
                        }
                    }
                    if (overlap) ++count;
                }
                return count;
            }

            static ll dist_lb_f(const array<array<ll, 2>, 4>& rng)
            {
                auto min_abs = [](ll a, ll b) {
                    if (a <= 0 && b >= 0) return 0ll;
                    return min(abs(a), abs(b));
                };
                ll dist = 0;
                for (auto [from, to] : rng) {
                    dist = max(dist, min_abs(from, to));
                }
                return dist;
            }

            search_cube_t(const array<array<ll, 2>, 4>& r, const vector<array<ll, 5>>& bc) :
                rng(r), bots_ub(bots_ub_f(r, bc)), dist_lb(dist_lb_f(r))
            {
                max_side = 0;
                for (auto [from, to] : r) {
                    max_side = max(max_side, to - from + 1);
                }
            }

            search_cube_t() = default;

            array<search_cube_t, 2> split(const vector<array<ll, 5>>& bc) const
            {
                for (size_t i = 0; i < rng.size(); ++i) {
                    if (rng[i][1] - rng[i][0] + 1 == max_side) {
                        ll lo = rng[i][0];
                        ll hi = rng[i][1];
                        ll mid = lo + (hi - lo) / 2;
                        array<array<ll, 2>, 4> r0 = rng;
                        array<array<ll, 2>, 4> r1 = rng;
                        r0[i][0] = mid + 1;
                        r1[i][1] = mid;
                        return {search_cube_t(r0, bc), search_cube_t(r1, bc)};
                    }
                }
                CHECK(false);
                return {search_cube_t(), search_cube_t()};  // shut the compiler up
            }

            bool is_point() const { return max_side == 1; }

            bool is_integer() const
            {
                // Only valid if the range has collapsed to a point
                CHECK(is_point());
                return (rng[0][0] & 1) == (rng[1][0] & 1) && (rng[0][0] & 1) == (rng[2][0] & 1) &&
                    (rng[0][0] & 1) == (rng[3][0] & 1);
            }

            bool operator<(const search_cube_t& rhs) const
            {
                return bots_ub < rhs.bots_ub ||
                    (bots_ub == rhs.bots_ub &&
                     (dist_lb > rhs.dist_lb ||
                      (dist_lb == rhs.dist_lb && max_side > rhs.max_side)));
            }

            array<array<ll, 2>, 4> rng;
            int bots_ub;
            ll dist_lb;
            ll max_side;
        };

        priority_queue<search_cube_t> queue;
        queue.push(search_cube_t(bounds, bot_cubes));
        int best_bots = 0;
        ll best_dist = LLONG_MAX;
        while (!queue.empty()) {
            search_cube_t cube = queue.top();
            queue.pop();
            if (cube.is_point()) {
                if (!cube.is_integer()) continue;
                if (cube.bots_ub > best_bots ||
                    (cube.bots_ub == best_bots && cube.dist_lb < best_dist))
                {
                    best_bots = cube.bots_ub;
                    best_dist = cube.dist_lb;
                }
                continue;
            }
            auto [c0, c1] = cube.split(bot_cubes);
            CHECK(c0.max_side >= 1);
            CHECK(c1.max_side >= 1);
            if (c0.bots_ub > best_bots || (c0.bots_ub == best_bots && c0.dist_lb < best_dist)) {
                queue.push(c0);
            }
            if (c1.bots_ub > best_bots || (c1.bots_ub == best_bots && c1.dist_lb < best_dist)) {
                queue.push(c1);
            }
        }
        CHECK(best_dist != LLONG_MAX);
        part2 = best_dist;
    }

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
