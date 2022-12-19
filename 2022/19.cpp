/*
 * Problem 19, Advent of Code 2022
 * Danjel McGougan
 * 
 * Uses C++20 features
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string>
split(const std::string& str, const std::string& sep)
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

// Blueprints with resource costs
struct blueprint {
    ll r_ore_ore;
    ll r_clay_ore;
    ll r_obs_ore;
    ll r_obs_clay;
    ll r_geo_ore;
    ll r_geo_obs;
    ll max_r_ore;
};

// Current search state
struct state {
    explicit state(uint8_t minutes) : m(minutes) { }
    auto operator<=>(const state& rhs) const = default;

    uint8_t m;
    uint8_t ore = 0;
    uint8_t clay = 0;
    uint8_t obs = 0;
    uint8_t r_clay = 0;
    uint8_t r_obs = 0;
    uint8_t r_ore = 1;
};

static map<state, uint8_t> cache;
static vector<blueprint> blueprints;
static blueprint bp;

static inline void update_state(state& s)
{
    s.m--;
    s.ore += s.r_ore;
    s.clay += s.r_clay;
    s.obs += s.r_obs;

    // Reduce the state space to increase efficiency of the cache
    s.ore = min(s.m * bp.max_r_ore, ll(s.ore));
    s.clay = min(s.m * bp.r_obs_clay, ll(s.clay));
    if (s.r_ore >= bp.max_r_ore) {
        s.ore = 2 * bp.max_r_ore;
    }
    if (s.r_clay >= bp.r_obs_clay) {
        s.clay = 2 * bp.r_obs_clay;
    }
}

static ll dfs(state s, ll geo)
{
    if (s.m == 1) return geo;

    auto iter = cache.find(s);
    if (iter != cache.end()) {
        return iter->second + geo;
    }

    ll max_geo = 0;
    ll can_build = 0;

    // Check if we have the resources to build a geode robot
    if (s.ore >= bp.r_geo_ore && s.obs >= bp.r_geo_obs) {
        state ns = s;
        update_state(ns);
        ns.ore -= bp.r_geo_ore;
        ns.obs -= bp.r_geo_obs;
        max_geo = max(max_geo, dfs(ns, geo + ns.m));
        // If we have the resources to build a geode robot we should always do it.
        // There is no reason to do anything else, so skip other branches of the tree.
        goto exit;
    }

    // Shortcut if we are reaching the end; if the time is 2 and we are not building
    // a geode robot, then no more geodes can be produced.
    if (s.m == 2) {
        max_geo = geo;
        goto exit;
    }

    // Check if we have the resources to build an obsidian robot
    if (s.ore >= bp.r_obs_ore && s.clay >= bp.r_obs_clay) {
        state ns = s;
        update_state(ns);
        ns.ore -= bp.r_obs_ore;
        ns.clay -= bp.r_obs_clay;
        ns.r_obs++;
        can_build++;
        max_geo = max(max_geo, dfs(ns, geo));
    }

    // Check if we have the resources to build a clay robot
    if (s.ore >= bp.r_clay_ore) {
        state ns = s;
        update_state(ns);
        ns.ore -= bp.r_clay_ore;
        ns.r_clay++;
        can_build++;
        max_geo = max(max_geo, dfs(ns, geo));
    }

    // Check if we have the resources to build an ore robot.
    // If we already have enough ore robots to produce ore for either
    // a clay, obsidian or geode robot in one turn then we skip
    // this part of the search. There is no reason to build more.
    if (s.ore >= bp.r_ore_ore && s.r_ore < bp.max_r_ore) {
        state ns = s;
        update_state(ns);
        ns.ore -= bp.r_ore_ore;
        ns.r_ore++;
        can_build++;
        max_geo = max(max_geo, dfs(ns, geo));
    }

    // We always have the option to do nothing.
    // But if all robots can be built then we should build one
    // of them since we are otherwise just wasting time.
    if (can_build < 3) {
        state ns = s;
        update_state(ns);
        max_geo = max(max_geo, dfs(ns, geo));
    }

exit:
    cache.insert(make_pair(s, max_geo - geo));

    return max_geo;
}

int main()
{
    // Parse the input
    string line;
    while (getline(cin, line)) {
        auto words = split(line, " ");
        assert(words.size() == 32);
        blueprint p;
        stringstream(words[6]) >> p.r_ore_ore;
        stringstream(words[12]) >> p.r_clay_ore;
        stringstream(words[18]) >> p.r_obs_ore;
        stringstream(words[21]) >> p.r_obs_clay;
        stringstream(words[27]) >> p.r_geo_ore;
        stringstream(words[30]) >> p.r_geo_obs;
        p.max_r_ore = p.r_ore_ore;
        p.max_r_ore = max(p.max_r_ore, p.r_clay_ore);
        p.max_r_ore = max(p.max_r_ore, p.r_obs_ore);
        p.max_r_ore = max(p.max_r_ore, p.r_geo_ore);
        blueprints.push_back(p);
    }

    ll q = 0;
    for (ll i = 0; i < ll(blueprints.size()); ++i) {
        bp = blueprints[i];
        cache.clear();
        state s(24);
        ll max = dfs(s, 0);
        cout << '.';
        cout.flush();
        q += (i + 1) * max;
    }

    cout << "\nPart 1: " << q << endl;

    ll mult = 1;
    for (ll i = 0; i < 3 && i < ll(blueprints.size()); ++i) {
        bp = blueprints[i];
        cache.clear();
        state s(32);
        ll max = dfs(s, 0);
        cout << '.';
        cout.flush();
        mult *= max;
    }

    cout << "\nPart 2: " << mult << endl;

    return 0;
}
