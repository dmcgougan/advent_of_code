/*
 * Problem 24, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Hash function for array
template <typename T, size_t size>
struct std::hash<array<T, size>>
{
    size_t operator()(const array<T, size>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main()
{
    // Parse input
    vector<string> list;
    string line;
    while (getline(cin, line)) {
        list.emplace_back(move(line));
    }

    // Solve part 1
    unordered_set<array<int, 2>> black;  // coordinates of black tiles
    for (const string& s : list) {
        int row = 0, col = 0;
        size_t i = 0;
        while (i < s.size()) {
            char c = s[i++];
            switch (c) {
            case 'e':
                ++col;
                break;
            case 'w':
                --col;
                break;
            case 's':
                assert(i < s.size());
                c = s[i++];
                if (c == 'e') {
                    // southeast
                    ++row;
                    if (row & 1) ++col;
                } else {
                    assert(c == 'w');
                    // southwest
                    if (row & 1) --col;
                    ++row;
                }
                break;
            case 'n':
                assert(i < s.size());
                c = s[i++];
                if (c == 'e') {
                    // northeast
                    --row;
                    if (row & 1) ++col;
                } else {
                    assert(c == 'w');
                    // northwest
                    if (row & 1) --col;
                    --row;
                }
                break;
            default:
                assert(false);
            }
        }
        auto iter = black.find({row, col});
        if (iter == black.end()) {
            black.insert({row, col});
        } else {
            black.erase(iter);
        }
    }
    int part1 = black.size();

    // Solve part 2
    unordered_map<array<int, 2>, int> count;
    vector<array<int, 2>> remove;
    for (int i = 0; i < 100; ++i) {
        count.clear();
        // Determine how many black neighbors all tiles have
        for (auto [row, col] : black) {
            static const array<array<int, 2>, 6> dirs[2] = {
                {{{0, 1}, {1, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, 1}}},
                {{{0, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}}}};
            for (auto [dr, dc] : dirs[row & 1]) {
                ++count[{row + dr, col + dc}];
            }
        }
        // Determine which black tiles to flip
        remove.clear();
        for (const auto& pos : black) {
            int c = count[pos];
            if (c == 0 || c > 2) {
                remove.push_back(pos);
            }
        }
        // Determine which white tiles to flip
        for (const auto& [pos, c] : count)
            if (c == 2) black.insert(pos);
        // Update
        for (const auto& pos : remove) black.erase(pos);
    }
    int part2 = black.size();

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
