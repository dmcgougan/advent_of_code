/*
 * Problem 1, Advent of Code 2018
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<int> list;
    for (int n; in >> n;) {
        list.push_back(n);
    }

    // Solve part 1
    int part1 = std::accumulate(list.begin(), list.end(), 0);

    // Solve part 2
    int part2 = 0;
    {
        unordered_set<int> seen;
        int freq = 0;
        seen.insert(freq);
        for (bool found = false; !found;) {
            for (int n : list) {
                freq += n;
                if (seen.count(freq)) {
                    part2 = freq;
                    found = true;
                    break;
                }
                seen.insert(freq);
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
