/*
 * Problem 14, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using ll = int64_t;

int main()
{
    static ll wall[1000][1000];
    constexpr int srcX = 500;
    constexpr int srcY = 0;
    ll maxY = 0;

    // Parse input data
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        ll fromX, fromY, toX, toY;
        bool first = true;
        while (getline(ss, word, ' ')) {
            if (isdigit(word[0])) {
                char* end;
                toX = strtol(word.c_str(), &end, 10);
                toY = strtol(end + 1, nullptr, 10);
                assert(toX < 1000 && toY < 1000);
                if (toY > maxY)
                    maxY = toY;
                if (!first) {
                    wall[toX][toY] = 1;
                    if (fromY == toY) {
                        for (int x = fromX; x != toX; x += (toX > fromX ? 1 : -1)) {
                            wall[x][fromY] = 1;
                        }
                    } else {
                        assert(fromX == toX);
                        for (int y = fromY; y != toY; y += (toY > fromY ? 1 : -1)) {
                            wall[fromX][y] = 1;
                        }
                    }
                }
                fromX = toX;
                fromY = toY;
                first = false;
            }
        }
    }

    // Simulate the falling sand
    bool donePart1 = false;
    ll count = 0;
    ll x, y;
    do {
        x = srcX;
        y = srcY;
        for (;;) {
            if (y == maxY && !donePart1) {
                // One piece of sand reached the last level of rock
                cout << "Part 1: " << count << endl;
                donePart1 = true;
            }
            if (y <= maxY && !wall[x][y + 1]) {
                y++;
            } else if (y <= maxY && !wall[x - 1][y + 1]) {
                x--;
                y++;
            } else if (y <= maxY && !wall[x + 1][y + 1]) {
                x++;
                y++;
            } else {
                // Comes to rest
                assert(x >= 1 && x < 999);
                wall[x][y] = 2;
                count++;
                break;
            }
        }
    } while (x != srcX || y != srcY);

    cout << "Part 2: " << count << endl;

    return 0;
}
