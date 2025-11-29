/*
 * Problem 20, Advent of Code 2021
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <climits>
#include <iostream>
#include <unordered_set>

using namespace std;

// Hash function for array template
template <typename T, size_t size>
struct std::hash<array<T, size>>
{
    size_t operator()(const array<T, size>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

struct image
{
    image() { }

    void set_pixel(int x, int y)
    {
        min_x = min(x, min_x);
        max_x = max(x, max_x);
        min_y = min(y, min_y);
        max_y = max(y, max_y);
        pixels.insert({x, y});
    }

    int get_pixel(int x, int y)
    {
        if (x < min_x || x > max_x || y < min_y || y > max_y) return outside_set ? 1 : 0;
        return pixels.count({x, y});
    }

    void enhance(const string& algo)
    {
        image result;
        for (int x = min_x - 1; x <= max_x + 1; ++x) {
            for (int y = min_y - 1; y <= max_y + 1; ++y) {
                int ix = get_pixel(x - 1, y - 1) << 8 | get_pixel(x, y - 1) << 7 |
                    get_pixel(x + 1, y - 1) << 6 | get_pixel(x - 1, y) << 5 | get_pixel(x, y) << 4 |
                    get_pixel(x + 1, y) << 3 | get_pixel(x - 1, y + 1) << 2 |
                    get_pixel(x, y + 1) << 1 | get_pixel(x + 1, y + 1);
                assert(ix < int(algo.size()));
                if (algo[ix] == '#') result.set_pixel(x, y);
            }
        }
        bool oset = outside_set;
        *this = result;
        outside_set = oset;
        if (!oset && algo[0] == '#') outside_set = true;
        if (oset && algo[511] == '.') outside_set = false;
    }

    int pixels_lit() const
    {
        assert(!outside_set);
        return pixels.size();
    }

    bool outside_set = false;
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    unordered_set<array<int, 2>> pixels;
};

int main()
{
    // Parse input
    string algo;
    string line;
    while (getline(cin, line) && !line.empty()) {
        algo += line;
    }
    image img;
    int y = 0;
    while (getline(cin, line)) {
        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == '#') img.set_pixel(int(x), y);
        }
        ++y;
    }

    // Enhance
    size_t i;
    for (i = 0; i < 2; i++) {
        img.enhance(algo);
    }
    int part1 = img.pixels_lit();
    for (; i < 50; i++) {
        img.enhance(algo);
    }
    int part2 = img.pixels_lit();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
