/*
 * Problem 3, Advent of Code 2021
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static int convert(const string& s)
{
    int value = 0;
    for (int i = s.size() - 1, bit = 1; i >= 0; i--, bit <<= 1) {
        if (s[i] == '1') value |= bit;
    }
    return value;
}

static int rating(const vector<int>& report, int high_bit, int k)
{
    int rows = report.size();
    vector<bool> deleted(rows);
    int left = rows;
    int value = 0;
    for (int bit = high_bit; bit > 0; bit >>= 1) {
        int one_count = 0;
        for (int r = 0; r < rows; r++) {
            if (deleted[r]) continue;
            if (report[r] & bit) one_count++;
        }
        int keep = k;
        if (one_count >= (left + 1) / 2) keep = 1 - k;
        for (int r = 0; r < rows; r++) {
            if (deleted[r]) continue;
            if (!!(report[r] & bit) == keep) {
                value = report[r];
            } else {
                deleted[r] = true;
                left--;
            }
        }
        if (left == 1) break;
    }
    return value;
}

int main()
{
    vector<int> report;
    string line;
    int bits = 0;
    while (getline(cin, line)) {
        bits = line.size();
        report.push_back(convert(line));
    }
    int rows = report.size();
    int high_bit = (1 << (bits - 1));

    // Part 1
    int gamma = 0;
    int epsilon = 0;
    for (int bit = high_bit; bit > 0; bit >>= 1) {
        int one_count = 0;
        for (int r = 0; r < rows; r++) {
            if (report[r] & bit) one_count++;
        }
        if (one_count > rows / 2) {
            gamma |= bit;
        } else {
            epsilon |= bit;
        }
    }

    // Part 2
    int oxygen = rating(report, high_bit, 1);
    int co2 = rating(report, high_bit, 0);

    cout << "Part 1: " << gamma * epsilon << endl;
    cout << "Part 2: " << oxygen * co2 << endl;

    return 0;
}
