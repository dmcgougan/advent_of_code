/*
 * Problem 5, Advent of Code 2024
 * Danjel McGougan
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

static map<int, set<int>> before;
static map<int, set<int>> after;

static bool check(const vector<int>& list)
{
    for (size_t i = 0; i < list.size(); i++) {
        int v1 = list[i];
        const set<int>& a = after[v1];
        const set<int>& b = before[v1];
        for (size_t j = 0; j < i; j++) {
            int v2 = list[j];
            if (a.count(v2)) return false;
        }
        for (size_t j = i + 1; j < list.size(); j++) {
            int v2 = list[j];
            if (b.count(v2)) return false;
        }
    }
    return true;
}

int main()
{
    string line;
    while (getline(cin, line) && !line.empty()) {
        stringstream ss(line);
        int v1, v2;
        char c;
        ss >> v1 >> c >> v2;
        before[v2].insert(v1);
        after[v1].insert(v2);
    }
    vector<vector<int>> lists;
    while (getline(cin, line)) {
        stringstream ss(line);
        int v;
        char c;
        vector<int> list;
        while (ss >> v) {
            ss >> c;
            list.push_back(v);
        }
        lists.emplace_back(move(list));
    }

    int part1 = 0;
    int part2 = 0;
    for (const auto& l : lists) {
        if (check(l)) {
            part1 += l[l.size() / 2];
        } else {
            vector<int> list(l);
        restart:
            for (size_t i = 0; i < list.size(); i++) {
                int v1 = list[i];
                const set<int>& a = after[v1];
                const set<int>& b = before[v1];
                for (size_t j = 0; j < i; j++) {
                    int v2 = list[j];
                    if (a.count(v2)) {
                        swap(list[i], list[j]);
                        goto restart;
                    }
                }
                for (size_t j = i + 1; j < list.size(); j++) {
                    int v2 = list[j];
                    if (b.count(v2)) {
                        swap(list[i], list[j]);
                        goto restart;
                    }
                }
            }
            part2 += list[list.size() / 2];
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
