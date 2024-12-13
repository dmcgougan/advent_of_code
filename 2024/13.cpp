/*
 * Problem 13, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using ll = int64_t;

// Extended Euclidean algorithm
static ll egcd(ll a, ll b, ll& x, ll& y)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

static ll solve(ll ax, ll bx, ll x, ll ay, ll by, ll y)
{
    // Check if we can rule out solutions
    ll a, b, g;
    if (y % egcd(ay, by, a, b) != 0 || x % (g = egcd(ax, bx, a, b)) != 0) {
        return 0;
    }

    // Find one solution to the X position (where a and/or b may be negative)
    ll f = x / (ax * a + bx * b);
    a *= f;
    b *= f;
    assert(a * ax + b * bx == x);

    // Try to modify a and b to match the Y position
    ll ma = bx / g;
    ll mb = ax / g;
    ll y_now = (a * ay + b * by);
    ll dy = ((a + ma) * ay + (b - mb) * by) - y_now;
    if (dy) {
        ll k = (y - y_now) / dy;
        a += k * ma;
        b -= k * mb;
    }

    // Check if we found a non-negative solution that also matches the Y position
    if (a >= 0 && b >= 0 && a * ay + b * by == y) {
        assert(a * ax + b * bx == x);
        // Found a solution; return the number of tokens
        return a * 3 + b;
    }

    return 0;
}

int main()
{
    ll part1 = 0;
    ll part2 = 0;
    string line;
    while (getline(cin, line)) {
        ll ax, ay;
        sscanf(line.c_str(), "Button A: X+%ld, Y+%ld", &ax, &ay);
        ll bx, by;
        getline(cin, line);
        sscanf(line.c_str(), "Button B: X+%ld, Y+%ld", &bx, &by);
        ll x, y;
        getline(cin, line);
        sscanf(line.c_str(), "Prize: X=%ld, Y=%ld", &x, &y);
        getline(cin, line);

        part1 += solve(ax, bx, x, ay, by, y);
        part2 += solve(ax, bx, x + 10000000000000, ay, by, y + 10000000000000);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
