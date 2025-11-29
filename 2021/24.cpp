/*
 * Problem 24, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>

using namespace std;

// First pass of my input
//  1 inp w
//  2 mul x 0
//  3 add x z
//  4 mod x 26
//  5 div z 1
//  6 add x 12
//  7 eql x w
//  8 eql x 0
//  9 mul y 0
// 10 add y 25
// 11 mul y x
// 12 add y 1
// 13 mul z y
// 14 mul y 0
// 15 add y w
// 16 add y 9
// 17 mul y x
// 18 add z y

// Translation to C code with A, B and C that vary between passes
// w = <digit>;
// x = z % 26;
// z /= A;
// x += B;
// x = (x == w);
// x = (x == 0);
// y = 25;
// y *= x;
// y += 1;
// z *= y;
// y = w;
// y += C;
// y *= x;
// z += y;

// Equivalent C code to the program in my input
static bool is_valid_ref(const string& m)
{
    static const int A[14]{1, 1, 1, 26, 26, 1, 1, 26, 1, 26, 1, 26, 26, 26};
    static const int B[14]{12, 12, 12, -9, -9, 14, 14, -10, 15, -2, 11, -15, -9, -3};
    static const int C[14]{9, 4, 2, 5, 1, 6, 11, 15, 7, 12, 15, 9, 12, 12};
    int w = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    for (size_t i = 0; i < m.size(); ++i) {
        w = m[i] - '0';
        x = z % 26;
        z /= A[i];
        x += B[i];
        x = (x == w);
        x = (x == 0);
        y = 25;
        y *= x;
        y += 1;
        z *= y;
        y = w;
        y += C[i];
        y *= x;
        z += y;
    }
    return z == 0;
}

// Deobfuscated version after analysis of the code above
static bool is_valid(const string& m)
{
    return m[2] == m[3] + 7 && m[1] == m[4] + 5 && m[6] == m[7] - 1 && m[8] == m[9] - 5 &&
        m[10] == m[11] && m[5] == m[12] + 3 && m[0] == m[13] - 6;
}

// Construct max and min model numbers
static string get_model(bool max)
{
    string m(14, '0');
    m[3] += max ? 2 : 1;
    m[4] += max ? 4 : 1;
    m[7] += max ? 9 : 2;
    m[9] += max ? 9 : 6;
    m[11] += max ? 9 : 1;
    m[12] += max ? 6 : 1;
    m[13] += max ? 9 : 7;
    m[2] = m[3] + 7;
    m[1] = m[4] + 5;
    m[6] = m[7] - 1;
    m[8] = m[9] - 5;
    m[10] = m[11];
    m[5] = m[12] + 3;
    m[0] = m[13] - 6;
    assert(is_valid(m));
    assert(is_valid_ref(m));
    return m;
}

int main()
{
    cout << "Part 1: " << get_model(true) << endl;
    cout << "Part 2: " << get_model(false) << endl;
    return 0;
}
