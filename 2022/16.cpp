/*
 * Problem 16, Advent of Code 2022
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const std::string& str, const std::string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    assert(!sep.empty());
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.emplace_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.emplace_back(move(s));
    }
    return result;
}

// Room info
struct room
{
    std::string name;
    ll rate = 0;
    bool opened = false;
    vector<room*> tunnels;
    vector<pair<room*, ll>> costs;
};

// Map of all rooms; rooms have pointers to the rooms they lead to
static map<std::string, room> rooms;

// Use BFS to find the costs to go to rooms with non-zero valve rates
// Do this for all rooms in the map
static void calc_costs()
{
    deque<pair<room*, ll>> q;
    set<room*> done;

    for (auto& p : rooms) {
        room* root = &p.second;
        done.clear();
        done.insert(root);
        q.push_back(make_pair(root, 0));
        while (!q.empty()) {
            room* r = q.front().first;
            ll cost = q.front().second + 1;
            q.pop_front();
            for (room* dst : r->tunnels) {
                if (!done.count(dst)) {
                    if (dst->rate > 0) {
                        root->costs.push_back(make_pair(dst, cost));
                    }
                    done.insert(dst);
                    q.push_back(make_pair(dst, cost));
                }
            }
        }
    }
}

static ll max_pressure;

// Simple recursive search. We only simulate the rooms with non-zero valves
// in them, since it is only there that we need to branch off into different
// directions of the search tree.
static void part1(room* r, ll steps_left, ll pressure = 0)
{
    if (pressure > max_pressure) {
        max_pressure = pressure;
    }

    for (auto [dst, cost] : r->costs) {
        if (!dst->opened && cost + 1 < steps_left) {
            // One step to open the valve and cost steps to go to the next room
            ll new_steps_left = steps_left - cost - 1;
            dst->opened = true;
            part1(dst, new_steps_left, pressure + dst->rate * new_steps_left);
            dst->opened = false;
        }
    }
}

// State for me and dumbo
static struct
{
    room* r;
    ll walking;
} state[2];

// For part 2 we simulate two agents walking around, and for each decision
// point we branch off into all possible directions recursively. The reason
// we need to simulate the walking between decisions is that the two
// agents reach decision points independently of each other and these
// do not necessarily happen at the same time.
// This code could probably be sped up a ton by cutting off fruitless
// branches of the search tree for example by keeping track of the number
// of remaining valves and their combined capacity and checking if the
// current search direction can not possibly reach a new pressure maximum.
// Or by employing some clever DP strategy.
// But it's fast enough to complete in a few seconds, so good enough.
static void part2(ll steps_left, ll pressure = 0)
{
    assert(steps_left > 0);

    if (pressure > max_pressure) {
        max_pressure = pressure;
    }

    // Remember these; we need to restore the state before returning
    ll w0 = state[0].walking;
    ll w1 = state[1].walking;

    if (state[0].walking && state[1].walking) {
        // Both are walking
        state[0].walking--;
        state[1].walking--;
        part2(steps_left - 1, pressure);
        state[0].walking = w0;
        state[1].walking = w1;
        return;
    }

    if (!state[0].walking && !state[1].walking) {
        // Both are ready to choose another path
        room* r0 = state[0].r;
        room* r1 = state[1].r;
        for (auto [dst0, cost0] : r0->costs) {
            if (!dst0->opened && cost0 + 1 < steps_left) {
                for (auto [dst1, cost1] : r1->costs) {
                    if (!dst1->opened && dst1 != dst0 && cost1 + 1 < steps_left) {
                        dst0->opened = true;
                        dst1->opened = true;
                        state[0].r = dst0;
                        state[0].walking = cost0;
                        state[1].r = dst1;
                        state[1].walking = cost1;
                        part2(steps_left - 1,
                              pressure + dst0->rate * (steps_left - cost0 - 1) +
                                  dst1->rate * (steps_left - cost1 - 1));
                        dst0->opened = false;
                        dst1->opened = false;
                    }
                }
            }
        }
        state[0].r = r0;
        state[1].r = r1;
        state[0].walking = w0;
        state[1].walking = w1;
        return;
    }

    // One is walking, the other is choosing
    ll i;
    if (state[0].walking) {
        // Walking through tunnels
        state[0].walking--;
        i = 1;
    }
    if (state[1].walking) {
        // Walking through tunnels
        state[1].walking--;
        i = 0;
    }

    // Choose another path for the one not walking
    room* r = state[i].r;
    for (auto [dst, cost] : r->costs) {
        if (!dst->opened && cost + 1 < steps_left) {
            dst->opened = true;
            state[i].r = dst;
            state[i].walking = cost;
            part2(steps_left - 1, pressure + dst->rate * (steps_left - cost - 1));
            dst->opened = false;
        }
    }
    state[i].r = r;

    state[0].walking = w0;
    state[1].walking = w1;
}

int main()
{
    // Parse input data
    string data;
    stringstream cin_ss;
    cin_ss << cin.rdbuf();
    data = cin_ss.str();
    vector<string> lines = split(data, "\n");
    for (string& line : lines) {
        vector<string> words = split(line, " ");
        room r;
        r.name = words[1];
        stringstream(words[4].substr(5)) >> r.rate;
        rooms[r.name] = r;
    }
    for (string& line : lines) {
        vector<string> words = split(line, " ");
        auto iter = rooms.find(words[1]);
        assert(iter != rooms.end());
        room& r = iter->second;
        for (ll i = 9; i < (ll)words.size(); ++i) {
            auto iter = rooms.find(words[i].substr(0, 2));
            assert(iter != rooms.end());
            r.tunnels.push_back(&iter->second);
        }
    }

    // Calculate traversing costs
    calc_costs();

    // Find room AA
    auto iter = rooms.find("AA");
    assert(iter != rooms.end());
    room* aa = &iter->second;

    // Run part 1
    max_pressure = 0;
    part1(aa, 30);
    cout << "Part 1: " << max_pressure << endl;

    // Run part 2
    max_pressure = 0;
    state[0].r = aa;
    state[1].r = aa;
    part2(26);
    cout << "Part 2: " << max_pressure << endl;

    return 0;
}
