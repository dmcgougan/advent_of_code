/*
 * Problem 10, Advent of Code 2019
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class ratio_t
{
public:
    ratio_t(int a, int b) : m_a(a), m_b(b)
    {
        // Reduce the ratio to its smallest form
        if (m_a && m_b) {
            int g = gcd(m_a, m_b);
            m_a /= g;
            m_b /= g;
        } else {
            m_a = (m_a > 0) ? 1 : (m_a < 0) ? -1 : 0;
            m_b = (m_b > 0) ? 1 : (m_b < 0) ? -1 : 0;
        }
    }

    void inc(int& a, int& b) const
    {
        a += m_a;
        b += m_b;
    }

    bool operator<(const ratio_t& rhs) const { return m_a * rhs.m_b < m_b * rhs.m_a; }
    bool operator>(const ratio_t& rhs) const { return m_a * rhs.m_b > m_b * rhs.m_a; }
    bool operator==(const ratio_t& rhs) const { return m_a == rhs.m_a && m_b == rhs.m_b; }

private:
    int m_a;
    int m_b;
};

int main()
{
    // Parse input
    vector<string> grid;
    for (string line; getline(cin, line);) {
        assert(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(move(line));
    }
    assert(!grid.empty());
    int rows = grid.size();
    int cols = grid[0].size();

    // Solve part 1
    int part1 = 0;
    int sr = 0;
    int sc = 0;
    for (int r0 = 0; r0 < rows; ++r0) {
        for (int c0 = 0; c0 < cols; ++c0) {
            if (grid[r0][c0] != '#') continue;
            // Found an asteroid a0; count how many other asteriods it can see
            int count = 0;
            for (int r1 = 0; r1 < rows; ++r1) {
                for (int c1 = 0; c1 < cols; ++c1) {
                    if (grid[r1][c1] != '#' || (r1 == r0 && c1 == c0)) continue;
                    // Found asteroid a1; get a direction vector from a0 to a1
                    // The ratio_t class above will reduce the vector to its smallest integer form
                    ratio_t ratio(r1 - r0, c1 - c0);
                    // Walk from a0 to a1 and see if we find any asteroids between them
                    for (int r = r0, c = c0;;) {
                        ratio.inc(r, c);
                        if (r == r1 && c == c1) {
                            // Reached a1 without finding another asteroid
                            ++count;
                            break;
                        }
                        if (grid[r][c] == '#') {
                            // Found an obstructing asteroid
                            break;
                        }
                    }
                }
            }
            if (count > part1) {
                part1 = count;
                sr = r0;
                sc = c0;
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    int vcount = 0;
    auto sweep_quadrant = [&](int q) -> void {
        // Sweep the laser through the specified quadrant
        // Do the horizontal/vertical direction first
        static const pair<int, int> dirs[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        auto [dr, dc] = dirs[q];
        for (int r = sr, c = sc;;) {
            r += dr;
            c += dc;
            if (r < 0 || r >= rows || c < 0 || c >= cols) break;
            if (grid[r][c] == '#') {
                // Vaporize
                grid[r][c] = '.';
                if (++vcount == 200) part2 = c * 100 + r;
                break;
            }
        }
        // Get vectors to all asteroids in the quadrant
        vector<ratio_t> ratios;
        static const array<int, 2> quad[] = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
        for (int r = sr + quad[q][0]; r >= 0 && r < rows; r += quad[q][0]) {
            for (int c = sc + quad[q][1]; c >= 0 && c < cols; c += quad[q][1]) {
                if (grid[r][c] == '#') ratios.emplace_back(r - sr, c - sc);
            }
        }
        // Sort the vectors in the correct order for the quadrant
        // (so the laser vaporizes stuff in a clockwise direction)
        if (q != 2) {
            sort(ratios.begin(), ratios.end());
        } else {
            sort(ratios.begin(), ratios.end(), greater<ratio_t>());
        }
        // Remove duplicate entries
        ratios.erase(unique(ratios.begin(), ratios.end()), ratios.end());
        // Shoot the laser in the directions determined by the sorted ratios
        for (const ratio_t& ratio : ratios) {
            // Find the asteriod to vaporize in the current direction
            for (int r = sr, c = sc;;) {
                ratio.inc(r, c);
                if (grid[r][c] == '#') {
                    // Vaporize
                    grid[r][c] = '.';
                    if (++vcount == 200) part2 = c * 100 + r;
                    break;
                }
            }
        }
    };
    while (vcount < 200) {
        for (int q = 0; q < 4; q++) sweep_quadrant(q);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
