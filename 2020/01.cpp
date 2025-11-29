/*
 * Problem 1, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<int> nums;
    int num;
    while (cin >> num) nums.push_back(num);

    // Find solutions
    int part1 = 0;
    int part2 = 0;
    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = i + 1; j < nums.size(); ++j) {
            if (nums[i] + nums[j] == 2020) {
                part1 = nums[i] * nums[j];
                if (part2) break;
            }
            if (part2) continue;
            for (size_t k = j + 1; k < nums.size(); ++k) {
                if (nums[i] + nums[j] + nums[k] == 2020) {
                    part2 = nums[i] * nums[j] * nums[k];
                    break;
                }
            }
        }
        if (part1 && part2) break;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
