/*
 * Problem 7, Advent of Code 2024
 * Danjel McGougan
 */

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using ll = int64_t;

static ll calc(const vector<pair<ll, vector<ll>>>& eq, int ops)
{
    ll calc_result = 0;
    for (const auto& p : eq) {
        size_t i;
        ll value = p.first;
        const vector<ll>& list = p.second;

        // Combination
        vector<int> comb(list.size() - 1);
        do {
            ll result = list[0];
            for (i = 0; i < comb.size(); i++) {
                ll num = list[i + 1];
                switch (comb[i]) {
                case 0:
                    // Addition
                    result += num;
                    break;
                case 1:
                    // Multiplication
                    result *= num;
                    break;
                case 2:
                    // Concatenation
                    if (num > 0) {
                        int digits[20];
                        size_t j = 0;
                        do {
                            digits[j++] = num % 10;
                            num /= 10;
                        } while (num > 0);
                        do {
                            result = (result * 10) + digits[--j];
                        } while (j > 0);
                    } else {
                        result *= 10;
                    }
                    break;
                }
                if (result > value) break;
            }

            // Check if we found a valid combination of operators
            if (result == value) {
                calc_result += result;
                break;
            }

            // Check next combination
            for (i = 0; i < comb.size(); i++) {
                comb[i]++;
                if (comb[i] == ops) {
                    comb[i] = 0;
                } else {
                    break;
                }
            }
        } while (i < comb.size());
    }
    return calc_result;
}

int main()
{
    vector<pair<ll, vector<ll>>> eq;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        vector<ll> nums;
        ll value, num;
        char c;
        ss >> value;
        ss >> c;
        while (ss >> num) {
            nums.push_back(num);
        }
        eq.emplace_back(value, move(nums));
    }

    cout << "Part 1: " << calc(eq, 2) << endl;
    cout << "Part 2: " << calc(eq, 3) << endl;

    return 0;
}
