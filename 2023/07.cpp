/*
 * Problem 7, Advent of Code 2023
 * Danjel McGougan
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static int c2i_map[256];
static int c2i_map_joker[256];

static inline int card_to_int(int card) { return c2i_map[card]; }

static inline int card_to_int_joker(int card) { return c2i_map_joker[card]; }

// Calculate a numeric hand strength for the given hand
static int hand_strength(string hand)
{
    int strength = 0;
    int card_count[13] = {0};

    for (char sc : hand) {
        int c = card_to_int(static_cast<int>(sc));
        strength += strength * 13 + c;
        ++card_count[c];
    }

    int three = 0;
    int pair = 0;
    for (size_t i = 0; i < 13; ++i) {
        // Five of a kind
        if (card_count[i] == 5) return 60000000 + strength;
        // Four of a kind
        if (card_count[i] == 4) return 50000000 + strength;
        // Three of a kind
        if (card_count[i] == 3) three++;
        // Pair
        if (card_count[i] == 2) pair++;
    }
    // Full house
    if (three && pair) return 40000000 + strength;
    // Three of a kind
    if (three) return 30000000 + strength;
    // Two pair
    if (pair > 1) return 20000000 + strength;
    // Pair
    if (pair) return 10000000 + strength;
    // High card
    return strength;
}

// Calculate a numeric hand strength for the given hand including jokers
static int hand_strength_joker(string hand)
{
    int strength = 0;
    int card_count[13] = {0};

    for (char sc : hand) {
        int c = card_to_int_joker(static_cast<int>(sc));
        strength += strength * 13 + c;
        ++card_count[c];
    }

    // card_count[0] is now the number of jokers in the hand

    int three = 0;
    int pair = 0;
    // Five of a kind
    for (size_t i = 1; i < 13; ++i) {
        if (card_count[i] + card_count[0] == 5) return 60000000 + strength;
    }
    // Four of a kind
    for (size_t i = 1; i < 13; ++i) {
        if (card_count[i] + card_count[0] == 4) return 50000000 + strength;
    }
    // Three of a kind
    for (size_t i = 1; i < 13; ++i) {
        if (card_count[i] + card_count[0] == 3) {
            three++;
            // Remove the cards we used, saving a joker if possible
            card_count[0] -= (3 - card_count[i]);
            card_count[i] = 0;
        }
    }
    // Pair
    for (size_t i = 1; i < 13; ++i) {
        if (card_count[i] + card_count[0] == 2) {
            pair++;
            // If we used a joker remove it
            card_count[0] = 0;
        }
    }
    // Full house
    if (three && pair) return 40000000 + strength;
    // Three of a kind
    if (three) return 30000000 + strength;
    // Two pair
    if (pair > 1) return 20000000 + strength;
    // Pair
    if (pair) return 10000000 + strength;
    // High card
    return strength;
}

int main()
{
    // Initialize the card to int maps
    static const string cards = "23456789TJQKA";
    for (size_t i = 0; i < cards.size(); ++i) {
        c2i_map[static_cast<int>(cards[i])] = i;
    }
    static const string cards_joker = "J23456789TQKA";
    for (size_t i = 0; i < cards_joker.size(); ++i) {
        c2i_map_joker[static_cast<int>(cards_joker[i])] = i;
    }

    // Parse input data
    struct bid_t
    {
        string hand;
        int bid;
    };
    vector<bid_t> bids;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string hand;
        ss >> hand;
        int bid;
        ss >> bid;
        bids.push_back({hand, bid});
    }

    // Part 1
    ll part1 = 0;
    vector<pair<int, int>> strengths;
    for (const bid_t& bid : bids) {
        strengths.push_back({hand_strength(bid.hand), bid.bid});
    }
    sort(strengths.begin(), strengths.end());
    for (size_t i = 0; i < strengths.size(); ++i) {
        part1 += (i + 1) * strengths[i].second;
    }

    // Part 2
    ll part2 = 0;
    strengths.clear();
    for (const bid_t& bid : bids) {
        strengths.push_back({hand_strength_joker(bid.hand), bid.bid});
    }
    sort(strengths.begin(), strengths.end());
    for (size_t i = 0; i < strengths.size(); ++i) {
        part2 += (i + 1) * strengths[i].second;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
