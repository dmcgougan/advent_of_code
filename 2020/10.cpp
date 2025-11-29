/*
 * Problem 10, Advent of Code 2020
 * Danjel McGougan
 */

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static vector<int> list;

// How many ways to arrange the adapters if we start on the specified index of the list
static int64_t ways(size_t start)
{
    static unordered_map<size_t, int64_t> cache;
    if (start + 1 >= list.size()) return 1;
    auto iter = cache.find(start);
    if (iter != cache.end()) return iter->second;
    int64_t c = 0;
    for (size_t i = start + 1; i < list.size() && list[i] - list[start] <= 3; ++i) {
        c += ways(i);
    }
    cache[start] = c;
    return c;
}

int main()
{
    // Parse input
    string line;
    for (int j; cin >> j;) {
        list.push_back(j);
    }

    // Add the charging adapter itself and sort the list
    list.push_back(0);
    sort(list.begin(), list.end());

    // Solve part 1
    int diff1 = 0;
    int diff3 = 0;
    for (size_t i = 0; i + 1 < list.size(); ++i) {
        int diff = list[i + 1] - list[i];
        diff1 += (diff == 1);
        diff3 += (diff == 3);
    }
    int part1 = diff1 * (diff3 + 1);

    // Solve part 2
    int64_t part2 = ways(0);

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
