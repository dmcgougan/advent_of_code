/*
 * Problem 25, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Convert from a SNAFU number
static ll from_snafu(const string& snafu)
{
    ll num = 0;
    ll pos = 1;
    for (ll i = snafu.size() - 1; i >= 0; --i) {
        switch (snafu[i]) {
        case '0':
            break;
        case '1':
            num += pos;
            break;
        case '2':
            num += 2 * pos;
            break;
        case '-':
            num -= pos;
            break;
        case '=':
            num -= 2 * pos;
            break;
        default:
            assert(false);
        }
        pos *= 5;
    }
    return num;
}

// Convert to a SNAFU number
static string to_snafu(ll num)
{
    // Generate the number in reverse
    vector<char> snafu;
    while (num > 0) {
        char c;
        switch (num % 5) {
        case 0:
            c = '0';
            break;
        case 1:
            c = '1';
            break;
        case 2:
            c = '2';
            break;
        case 3:
            c = '=';
            num += 5;
            break;
        case 4:
            c = '-';
            num += 5;
            break;
        default:
            assert(false);
        }
        snafu.push_back(c);
        num /= 5;
    }
    // Restore the correct order
    string ret;
    for (auto iter = snafu.rbegin(); iter != snafu.rend(); ++iter) {
        ret += *iter;
    }
    return ret;
}

int main()
{
    ll sum = 0;

    // Parse the input
    string line;
    while (getline(cin, line)) {
        sum += from_snafu(line);
    }

    cout << "Part 1: " << to_snafu(sum) << endl;
    cout << "Part 2: <click>" << endl;

    return 0;
}
