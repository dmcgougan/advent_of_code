/*
 * Problem 10, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using ll = int64_t;

static void drawPixel(ll& crt_x, ll x)
{
    // Draw current pixel
    if (crt_x == x - 1 || crt_x == x || crt_x == x + 1) {
        cout << '#';
    } else {
        cout << ' ';
    }
    if (++crt_x == 40) {
        crt_x = 0;
        cout << endl;
    }
}

int main()
{
    string line;

    ll cycle = 1;
    ll x = 1;
    ll signalStrength = 0;
    ll crt_x = 0;
    while (getline(cin, line)) {
        stringstream ss(line);
        string instr;
        assert(getline(ss, instr, ' '));

        drawPixel(crt_x, x);

        // Execute instruction
        if (instr == "noop") {
            cycle++;
        } else if (instr == "addx") {
            ll value;
            ss >> value;
            cycle++;
            drawPixel(crt_x, x);
            if ((cycle - 20) % 40 == 0) {
                signalStrength += cycle * x;
            }
            x += value;
            cycle++;
        } else {
            assert(false);
        }
        if ((cycle - 20) % 40 == 0) {
            signalStrength += cycle * x;
        }
    }

    cout << "Part 1: " << signalStrength << endl;

    return 0;
}
