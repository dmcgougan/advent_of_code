/*
 * Problem 11, Advent of Code 2022
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
//#include <map>
//#include <set>

using namespace std;
using ll = int64_t;

struct monkey
{
    list<ll> items;
    char op;
    ll value;
    ll test;
    ll monkey_true;
    ll monkey_false;
    ll inspect_count = 0;
};

int main()
{
    string line;
    vector<monkey> monkeys;
    vector<monkey> saved_monkeys;
    string str;

    // Read input file (very messy)
    do {
        monkey m;
        assert(getline(cin, line));
        assert(getline(cin, line));
        {
            stringstream ss(line.substr(line.find("items:") + 7));
            while (getline(ss, str, ' ')) {
                ll value = strtol(str.c_str(), nullptr, 10);
                m.items.push_back(value);
            }
        }
        assert(getline(cin, line));
        {
            stringstream ss(line.substr(line.find("old") + 4));
            assert(getline(ss, str, ' '));
            m.op = str[0];
            assert(getline(ss, str, ' '));
            if (str == "old") {
                m.op = '^';
                m.value = 2;
            } else {
                m.value = strtol(str.c_str(), nullptr, 10);
            }
        }
        assert(getline(cin, line));
        line = line.substr(line.find("by") + 3);
        m.test = strtol(line.c_str(), nullptr, 10);
        assert(getline(cin, line));
        line = line.substr(line.find("monkey") + 7);
        m.monkey_true = strtol(line.c_str(), nullptr, 10);
        assert(getline(cin, line));
        line = line.substr(line.find("monkey") + 7);
        m.monkey_false = strtol(line.c_str(), nullptr, 10);
        monkeys.push_back(m);
    } while (getline(cin, line));

    // Save for part 2 of the problem
    saved_monkeys = monkeys;

    // Throw items for 20 rounds
    for (ll round = 0; round < 20; round++) {
        for (monkey& m : monkeys) {
            for (ll& item : m.items) {
                // Do operation on the item
                m.inspect_count++;
                switch (m.op) {
                case '+':
                    item += m.value;
                    break;
                case '*':
                    item *= m.value;
                    break;
                case '^':
                    item *= item;
                    break;
                default:
                    assert(false);
                }
                // Divide by 3
                item /= 3;
                // Check which monkey to throw the item to
                ll destinationMonkey = (item % m.test == 0 ? m.monkey_true : m.monkey_false);
                monkeys[destinationMonkey].items.push_back(item);
            }
            m.items.clear();
        }
    }

    // Check which two monkeys inspected the most items
    vector<ll> counts;
    for (monkey& m : monkeys) {
        counts.push_back(m.inspect_count);
    }
    sort(counts.rbegin(), counts.rend());

    cout << "Part 1: " << counts[0] * counts[1] << endl;

    // Multiply all divisors together
    ll divProduct = 1;
    for (const monkey& m : monkeys) {
        divProduct *= m.test;
    }

    // Part 2 - do all maths on worry levels mod divProduct
    monkeys = saved_monkeys;
    for (ll round = 0; round < 10000; round++) {
        for (monkey& m : monkeys) {
            for (ll& item : m.items) {
                // Do operation on the item
                m.inspect_count++;
                switch (m.op) {
                case '+':
                    item = (item + m.value) % divProduct;
                    break;
                case '*':
                    item = (item * m.value) % divProduct;
                    break;
                case '^':
                    item = (item * item) % divProduct;
                    break;
                default:
                    assert(false);
                }
                // Check which monkey to throw the item to
                ll destinationMonkey = (item % m.test == 0 ? m.monkey_true : m.monkey_false);
                monkeys[destinationMonkey].items.push_back(item);
            }
            m.items.clear();
        }
    }

    // Check which two monkeys inspected the most items
    counts.clear();
    for (monkey& m : monkeys) {
        counts.push_back(m.inspect_count);
    }
    sort(counts.rbegin(), counts.rend());

    cout << "Part 2: " << counts[0] * counts[1] << endl;

    return 0;
}
