/*
 * Problem 24, Advent of Code 2024
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

struct wire
{
    explicit wire(const string& s)
    {
        assert(s.size() == 3);
        strcpy(name, s.c_str());
    }
    explicit wire(const char* s)
    {
        assert(strlen(s) == 3);
        if (s[0] == 'x' || s[0] == 'y' || s[0] == 'z') assert(isdigit(s[1]) && isdigit(s[2]));
        strcpy(name, s);
    }
    wire(char type, int index)
    {
        name[0] = type;
        name[1] = index / 10 + '0';
        name[2] = index % 10 + '0';
        name[3] = 0;
    }
    wire()
    {
        name[0] = 0;
        name[1] = 0;
        name[2] = 0;
        name[3] = 0;
    }
    char type() const
    {
        char t = name[0];
        if (t == 'x' || t == 'y' || t == 'z') return t;
        return 'i';
    }
    int index() const
    {
        assert(type() != 'i');
        return (name[1] - '0') * 10 + (name[2] - '0');
    }
    bool operator<(const wire& rhs) const { return value < rhs.value; }
    bool operator==(const wire& rhs) const { return value == rhs.value; }
    union
    {
        char name[4];
        uint32_t value;
    };
};

struct gate
{
    gate(const wire& in1_, const string& op_, const wire& in2_, const wire& out_) :
        in1(in1_), in2(in2_), out(out_)
    {
        if (op_ == "AND") {
            op = '&';
        } else if (op_ == "OR") {
            op = '|';
        } else {
            assert(op_ == "XOR");
            op = '^';
        }
    }
    int execute(int v1, int v2) const
    {
        switch (op) {
        case '&':
            return v1 & v2;
        case '|':
            return v1 | v2;
        case '^':
            return v1 ^ v2;
        }
        assert(false);
    }
    wire in1;
    char op;
    wire in2;
    wire out;
};

// Find dependencies
static set<wire> find_deps(const vector<gate>& gates, wire w)
{
    set<wire> deps;
    bool added;
    do {
        added = false;
        for (const gate& g : gates) {
            if (!deps.count(g.out) && g.out != w) continue;
            if (!deps.count(g.in1)) {
                deps.insert(g.in1);
                added = true;
            }
            if (!deps.count(g.in2)) {
                deps.insert(g.in2);
                added = true;
            }
        }
    } while (added);
    return deps;
}

static ll calc_output(const vector<gate>& gates, ll x, ll y)
{
    map<wire, int> values;

    // Run gates
    bool changed;
    do {
        changed = false;
        for (const gate& g : gates) {
            if (values.find(g.out) != values.end()) continue;

            int v1 = 0;
            char t1 = g.in1.type();
            if (t1 == 'x') v1 = (x >> g.in1.index()) & 1;
            if (t1 == 'y') v1 = (y >> g.in1.index()) & 1;
            if (t1 == 'i') {
                auto iter = values.find(g.in1);
                if (iter == values.end()) continue;
                v1 = iter->second;
            }

            int v2 = 0;
            char t2 = g.in2.type();
            if (t2 == 'x') v2 = (x >> g.in2.index()) & 1;
            if (t2 == 'y') v2 = (y >> g.in2.index()) & 1;
            if (t2 == 'i') {
                auto iter = values.find(g.in2);
                if (iter == values.end()) continue;
                v2 = iter->second;
            }

            // Do gate operation
            values[g.out] = g.execute(v1, v2);
            changed = true;
        }
    } while (changed);

    // Get output value
    ll z = 0;
    for (const auto& [wire, value] : values) {
        if (wire.type() == 'z') {
            z |= ((ll)value << wire.index());
        }
    }

    return z;
}

// Test if the specified bit works in the adder circuit
static bool test_bit(const vector<gate>& gates, int bit)
{
    // carry input: 0
    if (((calc_output(gates, 0, 0) >> bit) & 1) != 0) return false;
    if (bit < 45) {
        if (((calc_output(gates, 0, ((ll)1 << bit)) >> bit) & 1) != 1) return false;
        if (((calc_output(gates, ((ll)1 << bit), 0) >> bit) & 1) != 1) return false;
        if (((calc_output(gates, ((ll)1 << bit), ((ll)1 << bit)) >> bit) & 1) != 0) return false;
    }
    if (bit > 0) {
        // carry input: 1
        ll base = ((ll)1 << (bit - 1));
        if (((calc_output(gates, base, base) >> bit) & 1) != 1) return false;
        if (bit < 45) {
            if (((calc_output(gates, base | ((ll)1 << bit), base) >> bit) & 1) != 0) return false;
            if (((calc_output(gates, base, base | ((ll)1 << bit)) >> bit) & 1) != 0) return false;
            if (((calc_output(gates, base | ((ll)1 << bit), base | ((ll)1 << bit)) >> bit) & 1) !=
                1)
                return false;
        }
    }
    return true;
}

// Swap gate outputs until all bits work in the adder circuit
static vector<string> find_swaps(vector<gate>& gates, vector<string> swaps, int bit)
{
    if (bit == 46) return swaps;
    if (test_bit(gates, bit)) return find_swaps(gates, swaps, bit + 1);
    set<wire> good = find_deps(gates, wire('z', bit - 1));
    for (size_t i = 0; i < gates.size(); ++i) {
        for (size_t j = i + 1; j < gates.size(); ++j) {
            gate& g1 = gates[i];
            gate& g2 = gates[j];
            if (good.count(g1.out) || good.count(g2.out)) continue;
            swap(g1.out, g2.out);
            if (test_bit(gates, bit)) {
                swaps.emplace_back(g1.out.name);
                swaps.emplace_back(g2.out.name);
                vector<string> s = find_swaps(gates, swaps, bit + 1);
                if (!s.empty()) return s;
            }
            swap(g1.out, g2.out);
        }
    }
    return vector<string>();
}

int main()
{
    string line;
    ll x = 0;
    ll y = 0;
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        string s;
        getline(ss, s, ':');
        int v;
        ss >> v;
        wire w(s);
        if (w.type() == 'x') x |= ((ll)v << w.index());
        if (w.type() == 'y') y |= ((ll)v << w.index());
    }
    vector<gate> gates;
    while (getline(cin, line)) {
        stringstream ss(line);
        string s;
        getline(ss, s, ' ');
        wire in1(s);
        string op;
        getline(ss, op, ' ');
        getline(ss, s, ' ');
        wire in2(s);
        getline(ss, s, ' ');
        getline(ss, s, ' ');
        wire out(s);
        gates.emplace_back(in1, op, in2, out);
    }

    ll part1 = calc_output(gates, x, y);

    string part2;
    vector<string> swaps = find_swaps(gates, vector<string>(), 0);
    assert(!swaps.empty());
    sort(swaps.begin(), swaps.end());
    for (const string& s : swaps) {
        if (!part2.empty()) part2 += ',';
        part2 += s;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
