/*
 * Problem 21, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>

using namespace std;
using ll = int64_t;

// Define operator+= for pair
template <typename T1, typename T2>
pair<T1, T2>& operator+=(pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

// One step using the deterministic die in part 1
static void deterministic_step(int& die, int& die_count, int& pos, int& score)
{
    pos = (pos + die + (die + 1) % 100 + (die + 2) % 100 + 3) % 10;
    die = (die + 3) % 100;
    die_count += 3;
    score += pos + 1;
}

// Returns number of universes each player wins in given the initial state:
// position and score of each player, die sum and which die throw is about to happen (0 to 5)
static pair<ll, ll> quantum_wins(int pos_p1,
                                 int score_p1,
                                 int pos_p2,
                                 int score_p2,
                                 int dsum,
                                 int dthrow)
{
    static map<tuple<int, int, int, int, int, int>, pair<ll, ll>> cache;
    auto iter = cache.find({pos_p1, score_p1, pos_p2, score_p2, dsum, dthrow});
    if (iter != cache.end()) {
        return iter->second;
    }
    pair<ll, ll> universes;
    for (int d = 1; d <= 3; ++d) {
        int pp1 = pos_p1, sp1 = score_p1, pp2 = pos_p2, sp2 = score_p2;
        int ds = dsum;
        if (dthrow < 3) {
            // Player 1's turn
            ds += d;
            if (dthrow == 2) {
                pp1 = (pp1 + ds) % 10;
                ds = 0;
                sp1 += pp1 + 1;
                if (sp1 >= 21) {
                    universes += {1, 0};
                    continue;
                }
            }
        } else {
            // Player 2's turn
            ds += d;
            if (dthrow == 5) {
                pp2 = (pp2 + ds) % 10;
                ds = 0;
                sp2 += pp2 + 1;
                if (sp2 >= 21) {
                    universes += {0, 1};
                    continue;
                }
            }
        }
        universes += quantum_wins(pp1, sp1, pp2, sp2, ds, (dthrow + 1) % 6);
    }
    cache[{pos_p1, score_p1, pos_p2, score_p2, dsum, dthrow}] = universes;
    return universes;
}

int main()
{
    // Parse input
    string line;
    int pos_p1, pos_p2;
    getline(cin, line);
    assert(sscanf(line.c_str(), "Player 1 starting position: %d", &pos_p1) == 1);
    getline(cin, line);
    assert(sscanf(line.c_str(), "Player 2 starting position: %d", &pos_p2) == 1);

    // Run deterministic die
    int score_p1 = 0, score_p2 = 0;
    int pp1 = pos_p1 - 1,
        pp2 = pos_p2 - 1;  // let pos go from 0 to 9 and add 1 when using the values
    int die = 0;           // let die go from 0 to 99 and add 1 when using the value
    int die_count = 0;
    for (;;) {
        deterministic_step(die, die_count, pp1, score_p1);
        if (score_p1 >= 1000) break;
        deterministic_step(die, die_count, pp2, score_p2);
        if (score_p2 >= 1000) break;
    }
    int part1 = min(score_p1, score_p2) * die_count;

    // Quantum die
    pair<ll, ll> u = quantum_wins(pos_p1 - 1, 0, pos_p2 - 1, 0, 0, 0);
    ll part2 = max(u.first, u.second);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
