/*
 * Problem 21, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = int64_t;

// Go from one button to another on directional keypad
static ll search_dir(char src_btn, char dst_btn, int level)
{
    // Directional keypad layout
    static const vector<string> pad = {
        "X^A",
        "<v>",
    };
    // <row, col> of the keys
    static const map<char, pair<int, int>> posmap = {
        {'^', {0, 1}},
        {'A', {0, 2}},
        {'<', {1, 0}},
        {'v', {1, 1}},
        {'>', {1, 2}},
    };

    // Memoize
    static map<tuple<char, char, int>, ll> cache;
    auto iter = cache.find({src_btn, dst_btn, level});
    if (iter != cache.end()) {
        return iter->second;
    }

    if (level == 1) {
        // Number of human keypresses to go from one button to another on level 1
        static const map<char, map<char, int>> cost = {
            {'^', {{'^', 0}, {'A', 1}, {'<', 2}, {'v', 1}, {'>', 2}}},
            {'A', {{'^', 1}, {'A', 0}, {'<', 3}, {'v', 2}, {'>', 1}}},
            {'<', {{'^', 2}, {'A', 3}, {'<', 0}, {'v', 1}, {'>', 2}}},
            {'v', {{'^', 1}, {'A', 2}, {'<', 1}, {'v', 0}, {'>', 1}}},
            {'>', {{'^', 2}, {'A', 1}, {'<', 2}, {'v', 1}, {'>', 0}}},
        };
        ll len = cost.find(src_btn)->second.find(dst_btn)->second;
        cache[{src_btn, dst_btn, level}] = len;
        return len;
    }

    int rows = pad.size();
    int cols = pad[0].size();

    // Dijkstra

    // len, row, col, button-state of next level keypad
    priority_queue<tuple<ll, int, int, char>,
                   vector<tuple<ll, int, int, char>>,
                   greater<tuple<ll, int, int, char>>>
        queue;

    // row, col, button-state
    set<tuple<int, int, char>> visited;
    auto [sr, sc] = posmap.find(src_btn)->second;
    auto [er, ec] = posmap.find(dst_btn)->second;
    queue.push({0, sr, sc, 'A'});
    while (!queue.empty()) {
        auto [len, r, c, btn_state] = queue.top();
        queue.pop();
        if (visited.count({r, c, btn_state})) continue;
        visited.insert({r, c, btn_state});
        if (r == er && c == ec) {
            if (btn_state == 'A') {
                cache[{src_btn, dst_btn, level}] = len;
                return len;
            }
            // Go back to the A button
            queue.push({len + search_dir(btn_state, 'A', level - 1), r, c, 'A'});
        } else {
            static const int dirs[4][3] = {{0, 1, '>'}, {1, 0, 'v'}, {0, -1, '<'}, {-1, 0, '^'}};
            for (int d = 0; d < 4; d++) {
                int nr = r + dirs[d][0];
                int nc = c + dirs[d][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || pad[r][c] == 'X') continue;
                char btn = dirs[d][2];
                // Add one for the final A press
                queue.push({len + search_dir(btn_state, btn, level - 1) + 1, nr, nc, btn});
            }
        }
    }
    assert(false);
}

// Go from one button to another on the numeric keypad with specified number of robot levels
static ll search_num(char src_btn, char dst_btn, int levels)
{
    // Numeric keypad layout
    static const vector<string> pad = {"789", "456", "123", "X0A"};
    // <row, col> of the keys
    static const map<char, pair<int, int>> posmap = {
        {'7', {0, 0}},
        {'8', {0, 1}},
        {'9', {0, 2}},
        {'4', {1, 0}},
        {'5', {1, 1}},
        {'6', {1, 2}},
        {'1', {2, 0}},
        {'2', {2, 1}},
        {'3', {2, 2}},
        {'0', {3, 1}},
        {'A', {3, 2}},
    };
    int rows = pad.size();
    int cols = pad[0].size();

    // Dijkstra

    // len, row, col, button-state (of next level keypad)
    priority_queue<tuple<ll, int, int, char>,
                   vector<tuple<ll, int, int, char>>,
                   greater<tuple<ll, int, int, char>>>
        queue;
    // row, col, button-state
    set<tuple<int, int, char>> visited;
    auto [sr, sc] = posmap.find(src_btn)->second;
    auto [er, ec] = posmap.find(dst_btn)->second;
    queue.push({0, sr, sc, 'A'});
    while (!queue.empty()) {
        auto [len, r, c, btn_state] = queue.top();
        queue.pop();
        if (visited.count({r, c, btn_state})) continue;
        visited.insert({r, c, btn_state});
        if (r == er && c == ec) {
            if (btn_state == 'A') return len;
            // Go back to the A button
            queue.push({len + search_dir(btn_state, 'A', levels), r, c, 'A'});
        } else {
            static const int dirs[4][3] = {{0, 1, '>'}, {1, 0, 'v'}, {0, -1, '<'}, {-1, 0, '^'}};
            for (int d = 0; d < 4; d++) {
                int nr = r + dirs[d][0];
                int nc = c + dirs[d][1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || pad[r][c] == 'X') continue;
                char btn = dirs[d][2];
                // Add one for the final A press
                queue.push({len + search_dir(btn_state, btn, levels) + 1, nr, nc, btn});
            }
        }
    }
    assert(false);
}

int main()
{
    ll part1 = 0;
    ll part2 = 0;

    string line;
    while (getline(cin, line)) {
        ll len1 = 0;
        ll len2 = 0;

        // Start state
        char btn_state = 'A';
        for (char btn : line) {
            // Add one for the final A press
            len1 += search_num(btn_state, btn, 2) + 1;
            len2 += search_num(btn_state, btn, 25) + 1;
            btn_state = btn;
        }

        // Calculate sum of complexities
        int code = stoi(line.substr(0, 3));
        part1 += len1 * code;
        part2 += len2 * code;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
