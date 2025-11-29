/*
 * Problem 2, Advent of Code 2021
 * Danjel McGougan
 */

#include <iostream>
#include <string>

using namespace std;

int main()
{
    int depth1 = 0;
    int depth2 = 0;
    int pos = 0;
    int aim = 0;
    string cmd;
    int value;
    while ((cin >> cmd) && (cin >> value)) {
        if (cmd == "up") {
            depth1 -= value;
            aim -= value;
        } else if (cmd == "down") {
            depth1 += value;
            aim += value;
        } else if (cmd == "forward") {
            pos += value;
            depth2 += aim * value;
        }
    }

    cout << "Part 1: " << depth1 * pos << endl;
    cout << "Part 2: " << depth2 * pos << endl;

    return 0;
}
