/*
 * Problem 17, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using ll = int64_t;

// Rocks flipped vertically
constexpr uint8_t rocks[5][4] = {
    {
        0b11110000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {
        0b01000000,
        0b11100000,
        0b01000000,
        0b00000000,
    },
    {
        0b11100000,
        0b00100000,
        0b00100000,
        0b00000000,
    },
    {
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
    },
    {
        0b11000000,
        0b11000000,
        0b00000000,
        0b00000000,
    },
};

constexpr ll rock_heights[5] = {1, 3, 3, 4, 2};
constexpr ll rock_widths[5] = {4, 3, 3, 1, 2};

// Jet pattern
string jets;

// Chamber indexed by row. Lower row numbers are lower in chamber.
static uint8_t chamber[256];

// Chamber indexing; use wrapping indices since we do not need to
// keep older parts of the chamber around
static inline uint8_t& ch(ll i) { return chamber[i & 0xff]; }

// State
static ll height = 0;
static ll jet_ix = 0;

// Hash the top 8 entries of the chamber state
static ll hash_chamber_state()
{
    ll hash = 5381;

    for (ll i = height - 8; i < height; ++i) {
        hash = hash * 33 + ch(i);
    }

    return hash;
}

// Check if a rock position has overlap with other rocks
static inline bool has_overlap(ll rock_ix, ll rock_row, ll rock_col)
{
    return (ch(rock_row + 0) & (rocks[rock_ix][0] >> rock_col)) ||
        (ch(rock_row + 1) & (rocks[rock_ix][1] >> rock_col)) ||
        (ch(rock_row + 2) & (rocks[rock_ix][2] >> rock_col)) ||
        (ch(rock_row + 3) & (rocks[rock_ix][3] >> rock_col));
}

static void place_rock(ll rock_ix)
{
    // Only check walls; we are above the other rocks
    ll rock_col = 2;
    for (ll i = 0; i < 4; i++) {
        // Jet push
        if (jets[jet_ix] == '<') {
            if (rock_col > 0) rock_col--;
        } else {
            if (rock_col + rock_widths[rock_ix] < 7) rock_col++;
        }
        jet_ix = jet_ix + 1 < ll(jets.size()) ? jet_ix + 1 : 0;
    }

    // Check both walls and other rocks
    ll rock_row = height;
    while (rock_row > 0 && !has_overlap(rock_ix, rock_row - 1, rock_col)) {
        rock_row--;

        // Jet push
        if (jets[jet_ix] == '<') {
            if (rock_col > 0 && !has_overlap(rock_ix, rock_row, rock_col - 1)) rock_col--;
        } else {
            if (rock_col + rock_widths[rock_ix] < 7 &&
                !has_overlap(rock_ix, rock_row, rock_col + 1))
                rock_col++;
        }
        jet_ix = jet_ix + 1 < ll(jets.size()) ? jet_ix + 1 : 0;
    }

    // Add the rock to the chamber and update the height
    ch(rock_row + 0) |= (rocks[rock_ix][0] >> rock_col);
    ch(rock_row + 1) |= (rocks[rock_ix][1] >> rock_col);
    ch(rock_row + 2) |= (rocks[rock_ix][2] >> rock_col);
    ch(rock_row + 3) |= (rocks[rock_ix][3] >> rock_col);
    height = max(rock_row + rock_heights[rock_ix], height);

    // Clear top of chamber for a new rock
    ch(height) = 0;
    ch(height + 1) = 0;
    ch(height + 2) = 0;
    ch(height + 3) = 0;
}

// Cache of world states that we have seen
static map<pair<ll, ll>, pair<ll, ll>> seen_states;

int main()
{
    constexpr ll ROCKS = 1000000000000;
    ll added_height = 0;
    bool close = false;

    // Parse input data
    assert(getline(cin, jets));

    // Let the rocks fall!
    ll rock_ix = 0;
    for (ll count = 0; count < ROCKS; ++count) {
        if (count == 2022) {
            cout << "Part 1: " << height << endl;
        }

        place_rock(rock_ix);
        rock_ix = (rock_ix + 1) % 5;

        if (count > 2022 && !close) {
            // Check if we've seen this state before
            auto p = make_pair(hash_chamber_state(), jet_ix * 5 + rock_ix);
            auto iter = seen_states.find(p);
            if (iter != seen_states.end()) {
                // Yes! So now we have these conditions:
                //  * The chamber state is the same as before
                //  * The position in the jet sequence is the same as before (jet_ix)
                //  * The position in the rock sequence is the same as before (rock_ix)
                // We now know exactly how the chamber height and the rock count
                // change from one of these states to the next.
                // So we can just fast forward until we get close to the number of
                // rocks we want to simulate.

                // Fast forward
                // The added height is stored separately to not mess up the chamber
                // array After this we are close and can continue the simulation
                auto [seen_count, seen_height] = iter->second;
                ll delta_c = count - seen_count;
                ll delta_h = height - seen_height;
                ll skips = (ROCKS - count) / delta_c;
                count += skips * delta_c;
                added_height += skips * delta_h;
                close = true;
            } else {
                // We have not seen this state before; remember it
                seen_states[p] = make_pair(count, height);
            }
        }
    }

    cout << "Part 2: " << height + added_height << endl;

    return 0;
}
