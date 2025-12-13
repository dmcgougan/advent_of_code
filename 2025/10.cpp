/*
 * Problem 10, Advent of Code 2025
 * Danjel McGougan
 */

#include <bit>
#include <climits>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <source_location>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;
using ll = long long;

[[noreturn]] static void check_failed(unsigned int line)
{
    cerr << std::format("CHECK failed at line {}\n", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, source_location loc = source_location::current())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(loc.line());
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

// Class to hold a rational number
struct rational_t
{
    ll num = 0;
    ll den = 1;

    rational_t() = default;
    rational_t(ll n) : num(n), den(1) { }
    rational_t(ll n, ll d) : num(n), den(d)
    {
        CHECK(d != 0);
        if (den < 0) {
            den = -den;
            num = -num;
        }
        ll g = std::gcd(num, den);
        num /= g;
        den /= g;
    }

    bool is_zero() const { return num == 0; }

    rational_t operator+(const rational_t& o) const
    {
        ll n = num * o.den + o.num * den;
        ll d = den * o.den;
        return rational_t(n, d);
    }
    rational_t operator-(const rational_t& o) const
    {
        ll n = num * o.den - o.num * den;
        ll d = den * o.den;
        return rational_t(n, d);
    }
    rational_t operator*(const rational_t& o) const
    {
        ll n = num * o.num;
        ll d = den * o.den;
        return rational_t(n, d);
    }
    rational_t operator/(const rational_t& o) const
    {
        CHECK(!o.is_zero());
        ll n = num * o.den;
        ll d = den * o.num;
        return rational_t(n, d);
    }
    ll ceil() const
    {
        if (num >= 0) return (num + den - 1) / den;
        return num / den;
    }
    ll floor() const
    {
        if (den == 1) {
            return num;
        }
        if (num >= 0) {
            return num / den;
        }
        ll q = num / den;
        ll rem = num % den;
        if (rem) return q - 1;
        return q;
    }
};

struct rref_t
{
    vector<int> pivot_row;
    vector<int> pivot_col;
    vector<int> free_col;
    vector<vector<rational_t>> a;
};

// Gauss-Jordan algorithm to find the free variables
static rref_t gauss_jordan(const vector<uint32_t>& button, const vector<int>& ctarget)
{
    const int n = static_cast<int>(ctarget.size());
    const int m = static_cast<int>(button.size());
    CHECK(n != 0 && m != 0);

    // Build augmented matrix [A | t], size n x (m+1)
    vector<vector<rational_t>> a(n, vector<rational_t>(m + 1));
    for (int c = 0; c < n; ++c) {
        for (int j = 0; j < m; ++j) {
            if (button[j] & (1u << c)) a[c][j] = rational_t(1);
        }
        a[c][m] = rational_t(ctarget[c]);
    }

    vector<int> pivot_row(m, -1);  // pivot_row[col] = row index or -1
    int row = 0;
    for (int col = 0; col < m && row < n; ++col) {
        // Find pivot
        int sel = -1;
        for (int i = row; i < n; ++i) {
            if (!a[i][col].is_zero()) {
                sel = i;
                break;
            }
        }
        if (sel == -1) continue;

        std::swap(a[sel], a[row]);

        // Normalize pivot row
        rational_t piv = a[row][col];
        for (int j = col; j <= m; ++j) {
            a[row][j] = a[row][j] / piv;
        }

        // Eliminate this column in all other rows
        for (int i = 0; i < n; ++i) {
            if (i == row) continue;
            rational_t f = a[i][col];
            if (f.is_zero()) continue;
            for (int j = col; j <= m; ++j) {
                a[i][j] = a[i][j] - f * a[row][j];
            }
        }

        pivot_row[col] = row;
        ++row;
    }

    // Check for all-zero rows
    for (int i = 0; i < n; ++i) {
        bool all_zero = true;
        for (int j = 0; j < m; ++j) {
            if (!a[i][j].is_zero()) {
                all_zero = false;
                break;
            }
        }
        CHECK(!all_zero || a[i][m].is_zero());
    }

    // Build return value
    std::vector<int> pivot_col;
    std::vector<int> free_col;
    for (int col = 0; col < m; ++col) {
        (pivot_row[col] != -1 ? pivot_col : free_col).push_back(col);
    }
    rref_t rref;
    rref.pivot_row = std::move(pivot_row);
    rref.pivot_col = std::move(pivot_col);
    rref.free_col = std::move(free_col);
    rref.a = std::move(a);

    return rref;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<tuple<uint32_t, vector<uint32_t>, vector<int>>> machines;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " ");
        CHECK(s.size() >= 3 && s.back().size() > 0);
        auto& [lights, button, ctarget] = machines.emplace_back();
        for (size_t i = 1, b = 1; i + 1 < s[0].size(); ++i, b <<= 1) {
            if (s[0][i] == '#') lights |= b;
        }
        for (size_t i = 1; i + 1 < s.size(); ++i) {
            stringstream ss(s[i]);
            uint32_t& b = button.emplace_back();
            while (ss && ss.peek() != ')') {
                int bbit;
                ss.ignore(1);
                ss >> bbit;
                b |= (1u << bbit);
            }
        }
        stringstream ss(s[s.size() - 1]);
        while (ss && ss.peek() != '}') {
            ss.ignore(1);
            ss >> ctarget.emplace_back();
        }
    }
    CHECK(!machines.empty());

    // Solve part 1
    int part1 = 0;
    {
        for (const auto& [lights, buttons, _] : machines) {
            // BFS
            queue<pair<uint32_t, int>> q;
            unordered_set<uint32_t> seen;
            q.push({0, 0});
            seen.insert(0);
            int min_count = -1;
            while (!q.empty()) {
                auto [l, count] = q.front();
                q.pop();
                if (l == lights) {
                    min_count = count;
                    break;
                }
                for (uint32_t b : buttons) {
                    uint32_t next = l ^ b;
                    if (seen.count(next)) continue;
                    q.push({next, count + 1});
                    seen.insert(next);
                }
            }
            CHECK(min_count >= 0);
            part1 += min_count;
        }
    }

    // Solve part 2
    ll part2 = 0;
    for (const auto& [_, button, ctarget] : machines) {
        int bsize = button.size();
        int csize = ctarget.size();
        rref_t rref = gauss_jordan(button, ctarget);
        int k = rref.free_col.size();

        // Build the equations
        struct expr_t
        {
            explicit expr_t(int k) : coef(k) { }
            vector<rational_t> coef;
            rational_t rhs;
        };
        vector<expr_t> expr(bsize, expr_t(k));
        for (int p : rref.pivot_col) {
            int r = rref.pivot_row[p];
            expr[p].rhs = rref.a[r][bsize];
            for (int i = 0; i < k; ++i) {
                int f = rref.free_col[i];
                expr[p].coef[i] = rref.a[r][f];
            }
        }

        // Calculate max values for the free variables
        vector<int> max_y(k);
        for (int i = 0; i < k; ++i) {
            int f = rref.free_col[i];
            int min_target = INT_MAX;
            for (int c = 0; c < csize; ++c) {
                if (button[f] & (1u << c)) min_target = min(min_target, ctarget[c]);
            }
            CHECK(min_target != INT_MAX);
            max_y[i] = min_target;
            CHECK(max_y[i] >= 0);
        }

        // DFS to find values of the free variables
        ll best = LLONG_MAX;
        vector<ll> y(k);
        auto dfs = [&](auto self, int d) -> void {
            if (d == k) {
                // Check if we found a solution and calculate sum of all variables
                ll total = 0;
                for (int i = 0; i < k; ++i) {
                    if (y[i] > max_y[i]) return;
                    total += y[i];
                }
                for (int p : rref.pivot_col) {
                    rational_t val = expr[p].rhs;
                    for (int i = 0; i < k; ++i) {
                        val = val - expr[p].coef[i] * y[i];
                    }
                    if (val.den != 1) return;
                    if (val.num < 0) return;
                    total += val.num;
                }
                best = min(best, total);
                return;
            }

            // Calculate what interval we need to check for this variable
            ll lo = 0;
            ll hi = max_y[d];
            for (int p : rref.pivot_col) {
                rational_t cd = expr[p].coef[d];
                if (cd.is_zero()) continue;
                rational_t S_assigned;
                for (int i = 0; i < d; ++i) {
                    S_assigned = S_assigned + expr[p].coef[i] * y[i];
                }
                rational_t S_best_unassigned;
                for (int i = d + 1; i < k; ++i) {
                    rational_t ci = expr[p].coef[i];
                    if (ci.num >= 0) continue;
                    S_best_unassigned = S_best_unassigned + ci * max_y[i];
                }
                rational_t T = expr[p].rhs - S_assigned - S_best_unassigned;
                if (cd.num > 0) {
                    hi = min(hi, (T / cd).floor());
                } else {
                    lo = max(lo, (T / cd).ceil());
                }
                if (lo > hi) return;
            }

            // Check the interval
            for (ll v = lo; v <= hi; ++v) {
                y[d] = v;
                self(self, d + 1);
            }
        };

        dfs(dfs, 0);

        CHECK(best != LLONG_MAX);
        part2 += best;
    }

    // Output answers
    cout << std::format("Part 1: {}\n", part1);
    cout << std::format("Part 2: {}\n", part2);

    return 0;
}
