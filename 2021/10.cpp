/*
 * Problem 10, Advent of Code 2021
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

using namespace std;

int main()
{
    static const map<char, int> opening{{'(', 0}, {'[', 1}, {'{', 2}, {'<', 3}};
    static const map<char, int> closing{{')', 0}, {']', 1}, {'}', 2}, {'>', 3}};
    static const int illegal_score[] = {3, 57, 1197, 25137};

    int part1 = 0;
    vector<int64_t> scores;

    string line;
    while (getline(cin, line)) {
        stack<int> s;
        bool illegal = false;
        for (char ch : line) {
            auto iter = opening.find(ch);
            if (iter != opening.end()) {
                s.push(iter->second);
            } else {
                iter = closing.find(ch);
                assert(iter != closing.end());
                if (s.top() != iter->second) {
                    // Found illegal character; add the syntax error score
                    part1 += illegal_score[iter->second];
                    illegal = true;
                    break;
                }
                s.pop();
            }
        }
        if (illegal) continue;

        // Found an incomplete line; calculate the autocomplete score
        int64_t score = 0;
        while (!s.empty()) {
            score = 5 * score + s.top() + 1;
            s.pop();
        }
        scores.push_back(score);
    }

    // Find middle score
    sort(scores.begin(), scores.end());
    int64_t part2 = scores[scores.size() / 2];

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
