/*
 * Problem 2, Advent of Code 2018
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<string> ids;
    for (string line; getline(in, line);) {
        ids.push_back(std::move(line));
    }

    // Solve part 1
    int part1 = 0;
    {
        int two = 0;
        int three = 0;
        for (const string& id : ids) {
            unordered_map<char, int> hist;
            for (char c : id) ++hist[c];
            bool found_two = false;
            bool found_three = false;
            for (auto [_, count] : hist) {
                if (count == 2) found_two = true;
                if (count == 3) found_three = true;
            }
            two += found_two;
            three += found_three;
        }
        part1 = two * three;
    }

    // Solve part 2
    string part2;
    {
        for (size_t i = 0; i < ids.size(); ++i) {
            for (size_t j = i + 1; j < ids.size(); ++j) {
                const string& a = ids[i];
                const string& b = ids[j];
                if (a.size() != b.size()) break;
                int diffs = 0;
                int diff_pos = 0;
                for (size_t p = 0; p < a.size(); ++p) {
                    if (a[p] != b[p]) {
                        ++diffs;
                        diff_pos = p;
                    }
                    if (diffs > 1) break;
                }
                if (diffs == 1) {
                    part2 = a;
                    part2.erase(diff_pos, 1);
                }
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
