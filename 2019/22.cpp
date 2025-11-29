/*
 * Problem 22, Advent of Code 2019
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using ll = int64_t;

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
    istream& in = argc > 1 ? (file.open(argv[1]), file) : cin;
    assert(in);

    // Parse input
    enum op_t
    {
        DEAL_INTO,
        CUT,
        DEAL_WITH_INC,
    };
    vector<pair<op_t, int>> ops;
    for (string line; getline(in, line);) {
        vector<string> v = split(line, " ");
        assert(v.size() >= 2);
        if (v[0] == "deal") {
            if (v[1] == "into") {
                ops.push_back({DEAL_INTO, 0});
            } else {
                assert(v[1] == "with" && v.size() == 4);
                ops.push_back({DEAL_WITH_INC, atoi(v[3].c_str())});
            }
        } else {
            assert(v[0] == "cut");
            ops.push_back({CUT, atoi(v[1].c_str())});
        }
    }
    assert(!ops.empty());

    // Solve part 1
    int part1 = 0;
    {
        vector<int> deck(10007);
        int dsize = deck.size();
        for (int i = 0; i < dsize; ++i) {
            deck[i] = i;
        }
        for (auto [op, param] : ops) {
            if (op == DEAL_INTO) {
                reverse(deck.begin(), deck.end());
            } else if (op == CUT) {
                vector<int> old_deck = deck;
                int p = param >= 0 ? param : param + dsize;
                assert(p < dsize);
                for (int i = 0; i < dsize; ++i) {
                    deck[i] = old_deck[p];
                    p = (p + 1) % dsize;
                }
            } else {
                assert(op == DEAL_WITH_INC);
                vector<int> old_deck = deck;
                int p = 0;
                for (int i = 0; i < dsize; ++i) {
                    deck[p] = old_deck[i];
                    p = (p + param) % dsize;
                }
            }
        }
        for (int i = 0; i < dsize; ++i) {
            if (deck[i] == 2019) {
                part1 = i;
                break;
            }
        }
    }

    // Solve part 2
    ll part2 = 0;
    {
        // Extended GCD
        auto egcd = [](auto& self, ll a, ll b) -> tuple<ll, ll, ll> {
            if (b == 0) return {a, 1, 0};
            auto [g, x1, y1] = self(self, b, a % b);
            return {g, y1, x1 - (a / b) * y1};
        };
        // Modular multiplicative inverse
        auto mod_inv = [&](ll p, ll N) -> ll {
            auto [g, x, y] = egcd(egcd, p, N);
            assert(g == 1);
            x %= N;
            if (x < 0) x += N;
            return x;
        };
        // Modular multiplication (prevents overflow)
        auto mod_mul = [](ll a, ll b, ll N) -> ll {
            ll result = 0;
            a %= N;
            if (a < 0) a += N;
            if (b < 0) b += N;
            while (b) {
                result = b & 1 ? (result + a) % N : result;
                a = 2 * a % N;
                b >>= 1;
            }
            return result;
        };
        // Modular power
        auto mod_pow = [&](ll base, ll exp, ll N) -> ll {
            ll result = 1;
            base %= N;
            if (base < 0) base += N;
            while (exp) {
                result = exp & 1 ? mod_mul(result, base, N) : result;
                base = mod_mul(base, base, N);
                exp >>= 1;
            }
            return result;
        };

        //
        // Express a permutation as
        //   P(i) = (S + Ki) mod N
        // where
        //   P(i) is position in the old deck (zero based)
        //   i is position in the new deck
        //
        // "Deal into new stack" corresponds to:
        //   S = -1
        //   K = -1
        //
        // "Cut X" corresponds to:
        //   S = X
        //   K = 1
        //
        // "Deal with increment X" corresponds to:
        //   S = 0
        //   K = X_inv (multiplicative inverse of X mod N)
        //
        // We can chain permutations using the following rules:
        //   S_combined = (S_first + K_first * S_second) mod N
        //   K_combined = (K_first * K_second) mod N
        //
        const ll N = 119315717514047;
        ll S = 0;
        ll K = 1;
        for (auto [op, X] : ops) {
            if (op == DEAL_INTO) {
                S = (S - K) % N;
                K = -K;
            } else if (op == CUT) {
                S = (S + mod_mul(K, X, N)) % N;
            } else {
                assert(op == DEAL_WITH_INC);
                K = mod_mul(K, mod_inv(X, N), N);
            }
        }

        //
        // Now we have S and K for the complete shuffle procedure
        // To apply it T times we can use the following formula:
        //   S_T = (S * (K^T - 1) * (K - 1)_inv) mod N
        //   K_T = K^T mod N
        //
        const ll T = 101741582076661;
        S = mod_mul(S, mod_pow(K, T, N) - 1, N);
        S = mod_mul(S, mod_inv(K - 1, N), N);
        K = mod_pow(K, T, N);

        // Get the original position of the card at position 2020
        part2 = (S + mod_mul(K, 2020, N)) % N;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
