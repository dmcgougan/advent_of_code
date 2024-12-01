/*
 * Problem 1, Advent of Code 2024
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
namespace r = std::ranges;

int main()
{
    vector<int> llist;
    vector<int> rlist;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        int v1, v2;
        ss >> v1 >> v2;
        llist.push_back(v1);
        rlist.push_back(v2);
    }
    r::sort(llist);
    r::sort(rlist);

    int part1 = 0;
    int part2 = 0;
    for (size_t i = 0; i < llist.size(); i++) {
        part1 += abs(llist[i] - rlist[i]);
        part2 += llist[i] * r::count_if(rlist, [&](int v) { return v == llist[i]; });
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
