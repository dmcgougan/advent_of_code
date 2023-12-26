/*
 * Problem 24, Advent of Code 2023
 * Danjel McGougan
 *
 * This is my solution to part 1. I solved part 2 using Mathematica.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static constexpr ll lower_limit = 200000000000000;
static constexpr ll upper_limit = 400000000000000;

// Point or vector
struct ptv
{
    ptv(ll x_, ll y_) : x(x_), y(y_) { }
    ptv& operator+=(const ptv& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    bool operator==(const ptv& rhs) const { return x == rhs.x && y == rhs.y; }
    ll x;
    ll y;
};

static ptv operator+(ptv lhs, const ptv& rhs)
{
    lhs += rhs;
    return lhs;
}
static ll cross(const ptv& a, const ptv& b) { return (a.x * b.y) - (a.y * b.x); }

// ccw > 0:  a->b->c is a left turn
// ccw < 0:  a->b->c is a right turn
// ccw == 0: a->b->c is a line
static ll ccw(const ptv& a, const ptv& b, const ptv& c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Point and velocity vector
struct line
{
    line(ll px, ll py, ll vx, ll vy) : p(px, py), v(vx, vy) { }
    bool operator==(const line& rhs) const { return p == rhs.p && v == rhs.v; }
    ptv p;
    ptv v;
};

static bool intersect(line l1, line l2)
{
    ll c = cross(l1.v, l2.v);
    if (c == 0) {
        // Parallel lines - check that they do not intersect
        assert(ccw(l1.p, l1.p + l1.v, l2.p) != 0);
        return false;
    } else {
        // Order the lines so the cross product is positive
        if (c < 0) {
            swap(l1, l2);
            c = -c;
        }

        // Calculate line segment intersection
        // http://en.wikipedia.org/wiki/Line-line_intersection
        ll t_n = (l1.p.y - l2.p.y) * l2.v.x - (l1.p.x - l2.p.x) * l2.v.y;
        ll u_n = (l1.p.y - l2.p.y) * l1.v.x - (l1.p.x - l2.p.x) * l1.v.y;
        assert(c * l1.p.x + t_n * l1.v.x == c * l2.p.x + u_n * l2.v.x);
        assert(c * l1.p.y + t_n * l1.v.y == c * l2.p.y + u_n * l2.v.y);

        // Check if the intersection happens in the past for any of the lines
        if (t_n < 0 || u_n < 0) return false;

        // Check if the intersection happens inside the bounding square
        // The calculations overflow if we do them in 64 bit integer
        double x_c = (double)t_n * l1.v.x;
        double y_c = (double)t_n * l1.v.y;
        if (x_c < (double)c * (lower_limit - l1.p.x)) return false;
        if (x_c > (double)c * (upper_limit - l1.p.x)) return false;
        if (y_c < (double)c * (lower_limit - l1.p.y)) return false;
        if (y_c > (double)c * (upper_limit - l1.p.y)) return false;
        return true;
    }
}

int main()
{
    // Parse input data
    string str;
    vector<line> input;
    while (getline(cin, str)) {
        ll px, py, pz, vx, vy, vz;
        sscanf(str.c_str(), "%ld, %ld, %ld @ %ld, %ld, %ld", &px, &py, &pz, &vx, &vy, &vz);
        input.push_back(line(px, py, vx, vy));
    }
    int lines = input.size();

    int part1 = 0;
    for (int l1 = 0; l1 < lines; ++l1) {
        for (int l2 = l1 + 1; l2 < lines; ++l2) {
            if (intersect(input[l1], input[l2])) {
                ++part1;
            }
        }
    }

    cout << "Part 1: " << part1 << endl;

    return 0;
}
