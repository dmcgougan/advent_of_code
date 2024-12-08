/*
 * Problem 8, Advent of Code 2024
 * Danjel McGougan
 */

#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main()
{
    map<char, vector<pair<int, int>>> grid;
    string line;
    int rows = 0;
    int cols = 0;
    while (getline(cin, line)) {
        cols = 0;
        for (char antenna : line) {
            if (antenna != '.') {
                grid[antenna].emplace_back(rows, cols);
            }
            cols++;
        }
        rows++;
    }

    // Part 1
    set<pair<int, int>> antinodes;
    for (const auto& p : grid) {
        const auto& list = p.second;
        for (size_t i = 0; i + 1 < list.size(); i++) {
            for (size_t j = i + 1; j < list.size(); j++) {
                int dr = list[i].first - list[j].first;
                int dc = list[i].second - list[j].second;
                int r = list[i].first + dr;
                int c = list[i].second + dc;
                if (r >= 0 && r < rows && c >= 0 && c < cols) {
                    antinodes.emplace(r, c);
                }
                r = list[i].first - 2 * dr;
                c = list[i].second - 2 * dc;
                if (r >= 0 && r < rows && c >= 0 && c < cols) {
                    antinodes.emplace(r, c);
                }
            }
        }
    }
    int part1 = antinodes.size();

    // Part 2
    antinodes.clear();
    for (const auto& p : grid) {
        const auto& list = p.second;
        for (size_t i = 0; i + 1 < list.size(); i++) {
            for (size_t j = i + 1; j < list.size(); j++) {
                int dr = list[i].first - list[j].first;
                int dc = list[i].second - list[j].second;
                for (int f = 0;; f++) {
                    int r = list[i].first + f * dr;
                    int c = list[i].second + f * dc;
                    if (!(r >= 0 && r < rows && c >= 0 && c < cols)) break;
                    antinodes.emplace(r, c);
                }
                for (int f = 1;; f++) {
                    int r = list[i].first - f * dr;
                    int c = list[i].second - f * dc;
                    if (!(r >= 0 && r < rows && c >= 0 && c < cols)) break;
                    antinodes.emplace(r, c);
                }
            }
        }
    }
    int part2 = antinodes.size();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
