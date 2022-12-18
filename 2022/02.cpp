/*
 * Problem 2, Advent of Code 2022
 * Danjel McGougan
 */

#include <iostream>

using namespace std;
using ll = int64_t;

int main()
{
    string line;
    ll p1_score = 0;
    ll p2_score = 0;

    while (getline(cin, line)) {
        ll his_move = line[0] - 'A';
        ll my_move = line[2] - 'X';
        p1_score += my_move + 1 + (his_move == my_move) * 3 + ((my_move + 2) % 3 == his_move) * 6;

        ll result = line[2] - 'X';
        my_move = (his_move + result + 2) % 3;
        p2_score += my_move + 1 + result * 3;
    }

    cout << "Part 1: " << p1_score << endl;
    cout << "Part 2: " << p2_score << endl;

    return 0;
}
