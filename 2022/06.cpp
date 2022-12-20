/*
 * Problem 6, Advent of Code 2022
 * Danjel McGougan
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static bool checkMarker(const vector<char>& marker)
{
    for (size_t i = 0; i + 1 < marker.size(); i++) {
        for (size_t j = i + 1; j < marker.size(); j++) {
            if (marker[i] == marker[j]) {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    string line;

    vector<char> pktMarker;
    vector<char> msgMarker;
    bool foundPktMarker = false;
    char c;
    int count = 0;
    while (!(cin.get(c), cin.eof())) {
        count++;
        if (!foundPktMarker) {
            pktMarker.push_back(c);
            if (pktMarker.size() == 4) {
                if (checkMarker(pktMarker)) {
                    cout << "Part 1: " << count << endl;
                    foundPktMarker = true;
                }
                pktMarker.erase(pktMarker.begin());
            }
        }
        msgMarker.push_back(c);
        if (msgMarker.size() == 14) {
            if (checkMarker(msgMarker)) {
                cout << "Part 2: " << count << endl;
                break;
            }
            msgMarker.erase(msgMarker.begin());
        }
    }

    return 0;
}
