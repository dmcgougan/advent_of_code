/*
 * Problem 22, Advent of Code 2020
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <set>

using namespace std;

int main()
{
    // Parse input
    string line;
    array<deque<int>, 2> deck_orig;
    for (int i = 0; i < 2; ++i) {
        assert(getline(cin, line));
        while (getline(cin, line) && !line.empty()) {
            deck_orig[i].push_back(atoi(line.c_str()));
        }
    }

    // Solve part 1
    array<deque<int>, 2> deck(deck_orig);
    while (deck[0].size() && deck[1].size()) {
        int w = deck[1].front() > deck[0].front();
        deck[w].push_back(deck[w].front());
        deck[w].push_back(deck[w ^ 1].front());
        deck[w].pop_front();
        deck[w ^ 1].pop_front();
    }
    int w = !deck[0].size();
    int part1 = 0;
    for (size_t i = 0; i < deck[w].size(); ++i) {
        part1 += deck[w][i] * (deck[w].size() - i);
    }

    // Solve part 2
    deck = deck_orig;
    auto game = [&](auto& self, array<deque<int>, 2>& deck) -> int {
        set<array<deque<int>, 2>> seen;
        while (deck[0].size() && deck[1].size()) {
            if (seen.count(deck)) return 0;
            seen.insert(deck);
            int c[2] = {deck[0].front(), deck[1].front()};
            deck[0].pop_front();
            deck[1].pop_front();
            int w;
            if (int(deck[0].size()) >= c[0] && int(deck[1].size()) >= c[1]) {
                array<deque<int>, 2> copy;
                for (int p = 0; p < 2; ++p) {
                    for (int i = 0; i < c[p]; ++i) {
                        copy[p].push_back(deck[p][i]);
                    }
                }
                w = self(self, copy);
            } else {
                w = c[1] > c[0];
            }
            deck[w].push_back(c[w]);
            deck[w].push_back(c[w ^ 1]);
        }
        return !deck[0].size();
    };
    w = game(game, deck);
    int part2 = 0;
    for (size_t i = 0; i < deck[w].size(); ++i) {
        part2 += deck[w][i] * (deck[w].size() - i);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
