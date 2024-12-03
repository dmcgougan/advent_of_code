/*
 * Problem 3, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>

using namespace std;
using ll = int64_t;

static ll part1 = 0;
static ll part2 = 0;
static bool mul_enabled = true;

static void run_mul(char c)
{
    static int s = 0;
    static ll p1 = 0;
    static ll p2 = 0;
    static int p1_digits = 0;
    static int p2_digits = 0;
restart:
    switch (s) {
    case 0:
        if (c == 'm') {
            s = 1;
        }
        break;
    case 1:
        if (c == 'u') {
            s = 2;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 2:
        if (c == 'l') {
            s = 3;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 3:
        if (c == '(') {
            s = 4;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 4:
        if (c >= '0' && c <= '9') {
            p1 = p1 * 10 + c - '0';
            p1_digits++;
        } else if (c == ',' && p1_digits >= 1 && p1_digits <= 3) {
            s = 5;
        } else {
            p1_digits = 0;
            p1 = 0;
            s = 0;
            goto restart;
        }
        break;
    case 5:
        if (c >= '0' && c <= '9') {
            p2 = p2 * 10 + c - '0';
            p2_digits++;
        } else if (c == ')' && p2_digits >= 1 && p2_digits <= 3) {
            part1 += p1 * p2;
            if (mul_enabled) part2 += p1 * p2;
            p1_digits = 0;
            p2_digits = 0;
            p1 = 0;
            p2 = 0;
            s = 0;
        } else {
            p1_digits = 0;
            p2_digits = 0;
            p1 = 0;
            p2 = 0;
            s = 0;
            goto restart;
        }
        break;
    default:
        break;
    }
}

static void run_do(char c)
{
    static int s = 0;
restart:
    switch (s) {
    case 0:
        if (c == 'd') {
            s = 1;
        }
        break;
    case 1:
        if (c == 'o') {
            s = 2;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 2:
        if (c == '(') {
            s = 3;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 3:
        if (c == ')') {
            mul_enabled = true;
            s = 0;
        } else {
            s = 0;
            goto restart;
        }
        break;
    }
}

static void run_dont(char c)
{
    static int s = 0;
restart:
    switch (s) {
    case 0:
        if (c == 'd') {
            s = 1;
        }
        break;
    case 1:
        if (c == 'o') {
            s = 2;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 2:
        if (c == 'n') {
            s = 3;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 3:
        if (c == '\'') {
            s = 4;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 4:
        if (c == 't') {
            s = 5;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 5:
        if (c == '(') {
            s = 6;
        } else {
            s = 0;
            goto restart;
        }
        break;
    case 6:
        if (c == ')') {
            mul_enabled = false;
            s = 0;
        } else {
            s = 0;
            goto restart;
        }
        break;
    }
}

int main()
{
    char c;
    while (cin >> c) {
        run_do(c);
        run_dont(c);
        run_mul(c);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
