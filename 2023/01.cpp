/*
 * Problem 1, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    int part1 = 0;
    int part2 = 0;
    string line;

    while (getline(cin, line)) {
        int first = -1;
        int last = -1;
        size_t first_pos = string::npos;
        size_t last_pos = string::npos;
        for (size_t d = 0; d < 10; ++d) {
            char c = d + '0';
            size_t fpos = line.find(c);
            if (fpos < first_pos) {
                first_pos = fpos;
                first = d;
            }
            size_t lpos = line.rfind(c);
            if (lpos != string::npos && (last_pos == string::npos || lpos > last_pos)) {
                last_pos = lpos;
                last = d;
            }
        }
        part1 += first * 10 + last;

        static const vector<string> digit_str =
            {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
        for (size_t d = 0; d < digit_str.size(); ++d) {
            size_t fpos = line.find(digit_str[d]);
            if (fpos < first_pos) {
                first_pos = fpos;
                first = d + 1;
            }
            size_t lpos = line.rfind(digit_str[d]);
            if (lpos != string::npos && (last_pos == string::npos || lpos > last_pos)) {
                last_pos = lpos;
                last = d + 1;
            }
        }
        part2 += first * 10 + last;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
