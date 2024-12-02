/*
 * Problem 2, Advent of Code 2024
 * Danjel McGougan
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static bool check(const vector<int>& report)
{
    int prev = -1;
    int sign = 0;
    for (int v : report) {
        if (prev >= 0) {
            int diff = prev - v;
            int adiff = abs(diff);
            if (!sign) {
                sign = diff < 0 ? -1 : 1;
            }
            if (adiff < 1 || adiff > 3 || sign != (diff < 0 ? -1 : 1)) {
                return false;
            }
        }
        prev = v;
    }
    return true;
}

int main()
{
    int part1 = 0;
    int part2 = 0;

    string line;
    vector<int> report;
    while (getline(cin, line)) {
        stringstream ss(line);
        int v;
        while (ss >> v) {
            report.push_back(v);
        }
        if (check(report)) {
            // Report okay without removing any level
            part1++;
            part2++;
        } else {
            // Remove each level in turn and check if the report is okay
            for (size_t i = 0; i < report.size(); i++) {
                int v = report[i];
                report.erase(report.begin() + i);
                if (check(report)) {
                    // Removing one level makes the report okay
                    part2++;
                    break;
                }
                report.insert(report.begin() + i, v);
            }
        }
        report.clear();
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
