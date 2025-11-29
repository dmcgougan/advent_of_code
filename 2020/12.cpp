/*
 * Problem 12, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<pair<char, int>> instr;
    string line;
    while (getline(cin, line)) {
        assert(line.size() > 1);
        instr.push_back({line[0], atoi(line.substr(1).c_str())});
    }

    // Solve part 1
    static const pair<int, int> dirs[]{{1, 0}, {0, -1}, {-1, 0}, {0, 1}};  // E, S, W, N
    int x = 0, y = 0, d = 0;
    for (auto [action, value] : instr) {
        switch (action) {
        case 'N':
            y += value;
            break;
        case 'S':
            y -= value;
            break;
        case 'E':
            x += value;
            break;
        case 'W':
            x -= value;
            break;
        case 'L':
            d = (d + 4 - value / 90) % 4;
            break;
        case 'R':
            d = (d + value / 90) % 4;
            break;
        case 'F':
            x += value * dirs[d].first;
            y += value * dirs[d].second;
            break;
        default:
            assert(false);
        }
    }
    int part1 = abs(x) + abs(y);

    // Solve part 2
    x = 0;
    y = 0;
    int w_x = 10, w_y = 1;
    for (auto [action, value] : instr) {
        switch (action) {
        case 'N':
            w_y += value;
            break;
        case 'S':
            w_y -= value;
            break;
        case 'E':
            w_x += value;
            break;
        case 'W':
            w_x -= value;
            break;
        case 'L':
            for (int i = 0; i < value / 90; ++i) {
                // 90 degrees counter-clockwise
                // (for x increasing east and y increasing north)
                swap(w_x, w_y);
                w_x = -w_x;
            }
            break;
        case 'R':
            for (int i = 0; i < value / 90; ++i) {
                // 90 degrees clockwise
                // (for x increasing east and y increasing north)
                swap(w_x, w_y);
                w_y = -w_y;
            }
            break;
        case 'F':
            x += value * w_x;
            y += value * w_y;
            break;
        default:
            assert(false);
        }
    }
    int part2 = abs(x) + abs(y);

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
