/*
 * Problem 13, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

static void fold(vector<pair<int, int>>& dots, int fx, int fy)
{
    for (auto& [x, y] : dots) {
        if (x > fx) {
            x = fx - (x - fx);
        }
        if (y > fy) {
            y = fy - (y - fy);
        }
    }
}

static int count(const vector<pair<int, int>>& dots)
{
    set<pair<int, int>> s;
    for (const auto& p : dots) {
        s.insert(p);
    }
    return s.size();
}

static void print(const vector<pair<int, int>>& dots)
{
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;
    for (auto [x, y] : dots) {
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    vector<vector<bool>> matrix(max_x - min_x + 1, vector<bool>(max_y - min_y + 1, false));
    for (auto [x, y] : dots) {
        matrix[x - min_x][y - min_y] = true;
    }
    for (int y = 0; y <= max_y - min_y; ++y) {
        for (int x = 0; x <= max_x - min_x; ++x) {
            cout << (matrix[x][y] ? '#' : '.');
        }
        cout << endl;
    }
}

int main()
{
    vector<pair<int, int>> dots;
    vector<pair<int, int>> folds;
    string line;
    while (getline(cin, line) && !line.empty()) {
        stringstream s(line);
        int x, y;
        char comma;
        s >> x >> comma >> y;
        dots.push_back({x, y});
    }
    while (getline(cin, line)) {
        int v;
        char axis;
        assert(sscanf(line.c_str(), "fold along %c=%d", &axis, &v) == 2);
        folds.push_back({(axis == 'x' ? v : INT_MAX), (axis == 'y' ? v : INT_MAX)});
    }

    int part1 = 0;
    for (auto [fx, fy] : folds) {
        fold(dots, fx, fy);
        if (!part1) part1 = count(dots);
    }
    cout << "Part 1: " << part1 << endl;

    cout << "Part 2: " << endl;
    print(dots);

    return 0;
}
