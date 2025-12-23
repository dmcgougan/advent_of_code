/*
 * Problem 21, Advent of Code 2017
 * Danjel McGougan
 */

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
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

template <size_t N>
using img_t = array<array<char, N>, N>;
using img2_t = img_t<2>;
using img3_t = img_t<3>;
using img4_t = img_t<4>;

template <size_t N>
static void parse_img(const string& s, img_t<N>& image)
{
    for (size_t r = 0; r < N; ++r) {
        for (size_t c = 0; c < N; ++c) {
            size_t idx = r * (N + 1) + c;
            CHECK(idx < s.size());
            image[r][c] = s[idx];
        }
    }
}

template <size_t N>
static const img_t<N + 1>& find_rule(const vector<string>& img,
                                     int row,
                                     int col,
                                     const vector<pair<img_t<N>, img_t<N + 1>>>& rules)
{
    auto check = [&](const img_t<N>& p, unsigned t) {
        for (size_t r = 0; r < N; ++r) {
            for (size_t c = 0; c < N; ++c) {
                int nr = r;
                int nc = c;
                if (t & 1) swap(nr, nc);
                if (t & 2) nr = N - 1 - nr;
                if (t & 4) nc = N - 1 - nc;
                if (p[nr][nc] != img[row + r][col + c]) return false;
            }
        }
        return true;
    };
    auto is_match = [&](const img_t<N>& p) {
        if (check(p, 0)) return true;
        if (check(p, 1)) return true;
        if (check(p, 2)) return true;
        if (check(p, 3)) return true;
        if (check(p, 4)) return true;
        if (check(p, 5)) return true;
        if (check(p, 6)) return true;
        if (check(p, 7)) return true;
        return false;
    };
    for (const auto& [from, to] : rules) {
        if (is_match(from)) return to;
    }
    CHECK(false);
    unreachable();
}

template <size_t N>
static vector<string> enhance(const vector<string>& img,
                              const vector<pair<img_t<N>, img_t<N + 1>>>& rules)
{
    int tiles = img.size() / N;
    vector<string> nimg(tiles * (N + 1), string(tiles * (N + 1), 0));
    for (int tr = 0; tr < tiles; ++tr) {
        for (int tc = 0; tc < tiles; ++tc) {
            const img_t<N + 1>& tile = find_rule(img, tr * N, tc * N, rules);
            for (size_t r = 0; r < N + 1; ++r) {
                for (size_t c = 0; c < N + 1; ++c) {
                    nimg[tr * (N + 1) + r][tc * (N + 1) + c] = tile[r][c];
                }
            }
        }
    }
    return nimg;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<pair<img2_t, img3_t>> rules2;
    vector<pair<img3_t, img4_t>> rules3;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " => ");
        CHECK(s.size() == 2);
        if (s[0].size() == 5) {
            CHECK(s[1].size() == 11);
            auto& [i2, i3] = rules2.emplace_back();
            parse_img(s[0], i2);
            parse_img(s[1], i3);
        } else {
            CHECK(s[0].size() == 11 && s[1].size() == 19);
            auto& [i3, i4] = rules3.emplace_back();
            parse_img(s[0], i3);
            parse_img(s[1], i4);
        }
    }
    vector<string> img_init{".#.", "..#", "###"};

    // Common
    auto pixel_count = [&](int iterations) {
        vector<string> img = img_init;
        for (int i = 0; i < iterations; ++i) {
            if (img.size() % 2 == 0) {
                img = enhance(img, rules2);
            } else {
                CHECK(img.size() % 3 == 0);
                img = enhance(img, rules3);
            }
        }
        int count = 0;
        for (size_t r = 0; r < img.size(); ++r) {
            for (size_t c = 0; c < img.size(); ++c) {
                if (img[r][c] == '#') ++count;
            }
        }
        return count;
    };

    // Solve part 1
    int part1 = pixel_count(5);

    // Solve part 2
    int part2 = pixel_count(18);

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
