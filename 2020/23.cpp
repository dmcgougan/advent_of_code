/*
 * Problem 23, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static void round(vector<int>& cups, int& cur)
{
    size_t size = cups.size() - 1;

    // Remove three cups
    int c0 = cups[cur];
    int c1 = cups[c0];
    int c2 = cups[c1];
    cups[cur] = cups[c2];

    // Determine destination cup
    int dst = cur;
    do {
        dst = (dst + size - 2) % size + 1;
    } while (dst == c0 || dst == c1 || dst == c2);

    // Insert the three cups
    int next = cups[dst];
    cups[dst] = c0;
    cups[c0] = c1;
    cups[c1] = c2;
    cups[c2] = next;

    // Select new current
    cur = cups[cur];
}

int main()
{
    // Parse input
    string line;
    assert(getline(cin, line));

    // Solve part 1
    string part1;
    {
        // Construct the linked list
        vector<int> cups(line.size() + 1, 0);
        for (size_t i = 0; i < line.size(); ++i) {
            assert(line[i] >= '1' && line[i] <= '9');
            cups[line[i] - '0'] = line[(i + 1) % line.size()] - '0';
        }

        // Run rounds
        int cur = line[0] - '0';
        for (size_t i = 0; i < 100; ++i) {
            round(cups, cur);
        }

        // Get result
        cur = 1;
        for (size_t i = 0; i < line.size() - 1; i++) {
            cur = cups[cur];
            part1 += char(cur + '0');
        }
    }

    // Solve part 2
    int64_t part2;
    {
        // Construct the linked list
        vector<int> cups(1000001, 0);
        for (size_t i = 0; i < line.size() - 1; ++i) {
            cups[line[i] - '0'] = line[i + 1] - '0';
        }
        cups[line[line.size() - 1] - '0'] = line.size() + 1;
        for (size_t i = line.size() + 1; i < 1000000; ++i) {
            cups[i] = i + 1;
        }
        cups[1000000] = line[0] - '0';

        // Run rounds
        int cur = line[0] - '0';
        for (size_t i = 0; i < 10000000; ++i) {
            round(cups, cur);
        }

        // Get result
        part2 = cups[1];
        part2 *= cups[part2];
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
