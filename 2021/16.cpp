/*
 * Problem 16, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <iostream>

using namespace std;

static int get_bit(const string& hex, int bit_ix)
{
    assert(bit_ix / 4 < int(hex.size()));
    char digit = hex[bit_ix / 4];
    int num = (digit < 'A' ? digit - '0' : digit - 'A' + 10);
    return (num >> (3 - bit_ix % 4)) & 1;
}

static int get_field(const string& hex, int& bit_ix, int bit_len)
{
    int value = 0;
    for (int i = 0; i < bit_len; ++i) {
        value = 2 * value + get_bit(hex, bit_ix++);
    }
    return value;
}

static int part1;

static int64_t parse(const string& hex, int& bit_ix)
{
    int version = get_field(hex, bit_ix, 3);
    part1 += version;

    int type = get_field(hex, bit_ix, 3);
    assert(type >= 0 && type < 8);
    if (type == 4) {
        // Literal value
        int64_t value = 0;
        int field;
        do {
            field = get_field(hex, bit_ix, 5);
            value = 16 * value + field % 16;
        } while (field >= 16);
        return value;
    }

    // Operator
    int length_type = get_field(hex, bit_ix, 1);
    int limit = get_field(hex, bit_ix, length_type ? 11 : 15);
    int start_bit = bit_ix;
    static int64_t init[] = {0, 1, INT64_MAX, INT64_MIN, 0, -1, -1, -1};
    int64_t result = init[type];
    for (int p = 0; length_type ? p < limit : bit_ix < start_bit + limit; ++p) {
        int64_t value = parse(hex, bit_ix);
        switch (type) {
        case 0:
            // Sum
            result += value;
            break;
        case 1:
            // Product
            result *= value;
            break;
        case 2:
            // Minimum
            result = min(result, value);
            break;
        case 3:
            // Maximum
            result = max(result, value);
            break;
        case 5:
            // Greater than
            result = result >= 0 ? result > value : value;
            break;
        case 6:
            // Less than
            result = result >= 0 ? result < value : value;
            break;
        case 7:
            // Equal to
            result = result >= 0 ? result == value : value;
            break;
        }
    }

    return result;
}

int main()
{
    string hex;
    getline(cin, hex);

    int bit_ix = 0;
    int64_t part2 = parse(hex, bit_ix);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
