/*
 * Problem 8, Advent of Code 2018
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
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
    int lsize = list.size();

    // Solve part 1
    int part1 = 0;
    {
        auto meta_sum = [&](auto& self, int& pos) -> int {
            int sum = 0;
            assert(pos + 2 <= lsize);
            int children = list[pos++];
            int meta_entries = list[pos++];
            for (int c = 0; c < children; c++) {
                sum += self(self, pos);
            }
            assert(pos + meta_entries <= lsize);
            for (int m = 0; m < meta_entries; ++m) {
                sum += list[pos++];
            }
            return sum;
        };
        int pos = 0;
        part1 = meta_sum(meta_sum, pos);
    }

    // Solve part 2
    int part2 = 0;
    {
        auto value = [&](auto& self, int& pos) -> int {
            int value = 0;
            assert(pos + 2 <= lsize);
            int children = list[pos++];
            int meta_entries = list[pos++];
            vector<int> cvalues;
            for (int c = 0; c < children; c++) {
                cvalues.push_back(self(self, pos));
            }
            assert(pos + meta_entries <= lsize);
            for (int m = 0; m < meta_entries; ++m) {
                int entry = list[pos++];
                if (!children) {
                    value += entry;
                } else if (entry && entry <= children) {
                    value += cvalues[entry - 1];
                }
            }
            return value;
        };
        int pos = 0;
        part2 = value(value, pos);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
