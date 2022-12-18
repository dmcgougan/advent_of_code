/*
 * Problem 13, Advent of Code 2022
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

struct pkt {
    vector<pkt> l;
    ll i = -1;
    bool isInt() const { return i >= 0; }
};

// Recursive parser
static string pstr;
static size_t pos;
static vector<pkt>
parse_r()
{
    pkt p;
    vector<pkt> l;

    assert(pstr[pos++] == '[');
    while (pos < pstr.length() && pstr[pos] != ']') {
        char c = pstr[pos];
        if (c == '[') {
            p.i = -1;
            p.l = parse_r();
        } else {
            assert(isdigit((unsigned char)c));
            char* end;
            p.i = strtol(pstr.c_str() + pos, &end, 10);
            pos = end - pstr.c_str();
        }
        if (pstr[pos] == ',')
            pos++;
        l.push_back(p);
    }
    if (pos + 1 < pstr.length() && pstr[++pos] == ',')
        pos++;

    return l;
}

static pkt
parse(const string& s)
{
    pstr = s;
    pos = 0;
    return pkt { parse_r() };
}

static void
print(const pkt& p)
{
    if (p.isInt()) {
        cout << p.i;
    } else {
        bool first = true;
        cout << '[';
        for (const pkt& p : p.l) {
            if (!first)
                cout << ',';
            first = false;
            print(p);
        }
        cout << ']';
    }
}

static bool
same(const pkt& left, const pkt& right)
{
    if (left.isInt() && right.isInt()) {
        return left.i == right.i;
    }
    if (!left.isInt() && !right.isInt() && left.l.size() == right.l.size()) {
        auto liter = left.l.cbegin();
        auto riter = right.l.cbegin();
        while (liter != left.l.end()) {
            if (!same(*liter++, *riter++))
                return false;
        }
        return true;
    }
    return false;
}

static ll
order(const pkt& left, const pkt& right)
{
    if (left.isInt() && right.isInt()) {
        if (left.i == right.i)
            return 0;
        return left.i < right.i ? -1 : 1;
    }
    if (!left.isInt() && !right.isInt()) {
        auto liter = left.l.cbegin();
        auto riter = right.l.cbegin();
        while (liter != left.l.end() && riter != right.l.end()) {
            ll o = order(*liter++, *riter++);
            if (o != 0)
                return o;
        }
        if (liter == left.l.end() && riter == right.l.end())
            return 0;
        return liter == left.l.end() ? -1 : 1;
    }
    if (left.isInt()) {
        return order(pkt { { left } }, right);
    }
    return order(left, pkt { { right } });
}

int main()
{
    vector<pkt> packets;
    string line;
    ll index = 1;
    ll sum = 0;

    // Read input
    do {
        assert(getline(cin, line));
        pkt left = parse(line);
        packets.push_back(left);
        assert(getline(cin, line));
        pkt right = parse(line);
        packets.push_back(right);
        if (order(left, right) < 0)
            sum += index;
        index++;
    } while (getline(cin, line));

    // Insert divider packets
    pkt d1 = parse("[[2]]");
    pkt d2 = parse("[[6]]");
    packets.push_back(d1);
    packets.push_back(d2);

    // Sort the packets
    sort(packets.begin(), packets.end(), [](const pkt& l, const pkt& r) {
        return order(l, r) < 0;
    });

    // Find the divider packet indices
    ll m = 1;
    index = 1;
    for (const pkt& p : packets) {
        if (same(p, d1) || same(p, d2)) {
            m *= index;
        }
        index++;
    }

    cout << "Part 1: " << sum << endl;
    cout << "Part 2: " << m << endl;

    return 0;
}
