/*
 * Problem 15, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const std::string& str, const std::string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.emplace_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.emplace_back(move(s));
    }
    return result;
}

// Calculate the HASH of the given string
static int HASH(const string& s)
{
    int ret = 0;
    for (char c : s) {
        ret = (ret + (unsigned char)c) * 17;
    }
    return ret & 0xff;
}

int main()
{
    // Parse input data
    string line;
    getline(cin, line);
    vector<string> data = split(line, ",");

    // Part 1
    int part1 = 0;
    for (const string& s : data) {
        part1 += HASH(s);
    }

    // Part 2
    int part2 = 0;
    vector<vector<pair<string, int>>> boxes(256);
    for (const string& s : data) {
        stringstream ss(s);
        string label;
        int focal = 0;
        for (;;) {
            char c;
            ss >> c;
            if (c == '=') {
                ss >> focal;
                break;
            }
            if (c == '-') break;
            label.push_back(c);
        }
        int box = HASH(label);
        if (focal) {
            // Add lens with the given label and focal length to box
            bool found = false;
            for (auto& l : boxes[box]) {
                if (l.first == label) {
                    l.second = focal;
                    found = true;
                    break;
                }
            }
            if (!found) {
                boxes[box].push_back(make_pair(label, focal));
            }
        } else {
            // Remove lens with the given label from box
            for (auto iter = boxes[box].begin(); iter != boxes[box].end(); ++iter) {
                if (iter->first == label) {
                    boxes[box].erase(iter);
                    break;
                }
            }
        }
    }
    for (size_t i = 0; i < boxes.size(); ++i) {
        for (size_t j = 0; j < boxes[i].size(); ++j) {
            // Add the focal strength
            part2 += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
