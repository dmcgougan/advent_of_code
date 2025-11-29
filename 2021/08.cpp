/*
 * Problem 8, Advent of Code 2021
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Get the strings of the specified length from the vector
static vector<string> get_patterns(const vector<string>& patterns, size_t len)
{
    vector<string> v;
    for (const string& s : patterns) {
        if (s.size() == len) v.push_back(s);
    }
    return v;
}

int main()
{
    vector<vector<string>> patterns;
    vector<vector<string>> output;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string s;

        // Get the digit patterns
        patterns.emplace_back();
        for (int i = 0; i < 10; i++) {
            ss >> s;
            patterns.back().push_back(s);
        }

        // Eat pipe character
        ss >> s;

        // Get the output digits
        output.emplace_back();
        for (int i = 0; i < 4; i++) {
            ss >> s;
            output.back().push_back(s);
        }
    }
    size_t inputs = patterns.size();
    assert(inputs == output.size());

    // Digits
    // 0: length 6
    // 1: length 2
    // 2: length 5
    // 3: length 5
    // 4: length 4
    // 5: length 5
    // 6: length 6
    // 7: length 3
    // 8: length 7
    // 9: length 6

    // Segment indices
    //  0000
    // 1    2
    // 1    2
    //  3333
    // 4    5
    // 4    5
    //  6666

    int part1 = 0;
    int part2 = 0;
    for (size_t i = 0; i < inputs; i++) {
        // Count unique lengths for part 1
        for (const string& s : output[i]) {
            size_t len = s.size();
            // Part 1: Digits 1, 4, 7 and 8
            if (len == 2 || len == 4 || len == 3 || len == 7) part1++;
        }

        // Deduce the segments for part 2
        string seg(7, ' ');

        // Deduce segments 2 and 5
        auto s2 = get_patterns(patterns[i], 2);
        assert(s2.size() == 1);
        seg[2] = s2[0][0];
        seg[5] = s2[0][1];
        auto s6 = get_patterns(patterns[i], 6);
        assert(s6.size() == 3);
        for (const string& s : s6) {
            if (s.find_first_of(seg[5]) == string::npos) {
                // Found a pattern of length 6 that does not contain segment 5
                swap(seg[2], seg[5]);
                break;
            }
        }

        // Deduce segment 0
        auto s3 = get_patterns(patterns[i], 3);
        assert(s3.size() == 1);
        for (char c : s3[0]) {
            if (c != seg[2] && c != seg[5]) {
                seg[0] = c;
                break;
            }
        }
        assert(seg[0] != ' ');

        // Deduce segments 1 and 3
        auto s4 = get_patterns(patterns[i], 4);
        assert(s4.size() == 1);
        int p = 1;
        for (char c : s4[0]) {
            if (c != seg[2] && c != seg[5]) {
                seg[p] = c;
                p += 2;
            }
        }
        assert(p == 5);
        for (const string& s : s6) {
            if (s.find_first_of(seg[1]) == string::npos) {
                // Found a pattern of length 6 that does not contain segment 1
                swap(seg[1], seg[3]);
                break;
            }
        }

        // Deduce segments 4 and 6
        auto s7 = get_patterns(patterns[i], 7);
        assert(s7.size() == 1);
        p = 4;
        for (char c : s7[0]) {
            if (c != seg[0] && c != seg[1] && c != seg[2] && c != seg[3] && c != seg[5]) {
                seg[p] = c;
                p += 2;
            }
        }
        assert(p == 8);
        auto s5 = get_patterns(patterns[i], 5);
        assert(s5.size() == 3);
        for (const string& s : s5) {
            if (s.find_first_of(seg[6]) == string::npos) {
                // Found a pattern of length 5 that does not contain segment 6
                swap(seg[4], seg[6]);
                break;
            }
        }

        // Deduce the output digits for part 2
        static const vector<string> digits{"abcefg",
                                           "cf",
                                           "acdeg",
                                           "acdfg",
                                           "bcdf",
                                           "abdfg",
                                           "abdefg",
                                           "acf",
                                           "abcdefg",
                                           "abcdfg"};
        int place = 1000;
        for (const string& s : output[i]) {
            string d;
            for (char c : s) {
                size_t ix = seg.find_first_of(c);
                assert(ix != string::npos);
                d.push_back('a' + ix);
            }
            sort(d.begin(), d.end());
            auto it = find(digits.begin(), digits.end(), d);
            assert(it != digits.end());
            part2 += place * distance(digits.begin(), it);
            place /= 10;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
