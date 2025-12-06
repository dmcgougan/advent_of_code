/*
 * Problem 6, Advent of Code 2025
 * Danjel McGougan
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<vector<int>> nums;
    vector<char> ops;
    vector<string> nums_raw;
    string ops_raw;
    for (string line; getline(in, line);) {
        assert(!line.empty());
        stringstream ss(line);
        if (line[0] == '*' || line[0] == '+') {
            assert(!nums_raw.empty() && line.size() == nums_raw[0].size());
            ops_raw = line;
            char c;
            while (ss >> c) ops.push_back(c);
            break;
        }
        assert(nums_raw.empty() || line.size() == nums_raw[0].size());
        nums_raw.emplace_back(line);
        auto& v = nums.emplace_back();
        int num;
        while (ss >> num) v.push_back(num);
    }

    // Solve part 1
    int64_t part1 = 0;
    {
        for (size_t i = 0; i < nums[0].size(); ++i) {
            int64_t result = (ops[i] == '+' ? 0 : 1);
            for (size_t j = 0; j < nums.size(); ++j) {
                if (ops[i] == '+') {
                    result += nums[j][i];
                } else {
                    result *= nums[j][i];
                }
            }
            part1 += result;
        }
    }

    // Solve part 2
    int64_t part2 = 0;
    {
        vector<int> nums;
        for (size_t i = nums_raw[0].size(); i--;) {
            int64_t num = 0;
            // Go down the column and contruct the number
            for (size_t j = 0; j < nums_raw.size(); ++j) {
                if (nums_raw[j][i] != ' ') {
                    num = num * 10 + (nums_raw[j][i] - '0');
                }
            }
            nums.push_back(num);
            if (ops_raw[i] != ' ') {
                int64_t result;
                if (ops_raw[i] == '+') {
                    result = 0;
                    for (int n : nums) result += n;
                } else {
                    result = 1;
                    for (int n : nums) result *= n;
                }
                part2 += result;
                nums.clear();
                if (i) --i;  // Skip the blank column
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
