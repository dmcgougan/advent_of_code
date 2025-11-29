/*
 * Problem 16, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<int> input_signal;
    for (char ch; cin >> ch;) {
        assert(ch >= 0 && ch <= '9');
        input_signal.push_back(ch - '0');
    }

    // Solve part 1
    int part1 = 0;
    {
        vector<int> signal = input_signal;
        vector<int> next(signal.size(), 0);
        for (size_t i = 0; i < 100; ++i) {
            for (size_t p = 0; p < signal.size(); ++p) {
                unsigned state = !p;
                size_t count = !!p;
                int sum = 0;
                for (int digit : signal) {
                    sum += (state == 1) ? digit : 0;
                    sum -= (state == 3) ? digit : 0;
                    if (count < p) {
                        ++count;
                    } else {
                        count = 0;
                        state = (state + 1) % 4;
                    }
                }
                next[p] = abs(sum) % 10;
            }
            signal = next;
        }
        for (size_t p = 0; p < 8; ++p) {
            part1 = part1 * 10 + signal[p];
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Get the digit positions we are interested in (zero-based)
        size_t pos = 0;
        for (size_t i = 0; i < 7; ++i) pos = pos * 10 + input_signal[i];

        // Make sure it points somewhere in the last half of the signal
        assert(pos > 5000 * input_signal.size());

        // We only need to simulate the tail digits of the signal; from pos to end
        vector<uint8_t> signal;
        for (size_t i = pos; i < 10000 * input_signal.size(); ++i) {
            signal.push_back(input_signal[i % input_signal.size()]);
        }

        // Simulate 100 phases; a lot simpler in last half of the signal
        for (int phase = 0; phase < 100; ++phase) {
            int sum = 0;
            for (int i = signal.size() - 1; i >= 0; --i) {
                sum += signal[i];
                signal[i] = sum % 10;
            }
        }

        // Get the requested digits
        for (size_t p = 0; p < 8; ++p) {
            part2 = part2 * 10 + signal[p];
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
