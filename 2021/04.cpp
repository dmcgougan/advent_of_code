/*
 * Problem 4, Advent of Code 2021
 * Danjel McGougan
 */

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static bool has_won(const array<array<bool, 5>, 5>& mark)
{
    for (int r = 0; r < 5; r++) {
        int c;
        for (c = 0; c < 5; c++) {
            if (!mark[r][c]) break;
        }
        if (c == 5) return true;
    }
    for (int c = 0; c < 5; c++) {
        int r;
        for (r = 0; r < 5; r++) {
            if (!mark[r][c]) break;
        }
        if (r == 5) return true;
    }
    return false;
}

static int score(const array<array<bool, 5>, 5>& mark,
                 const array<array<int, 5>, 5>& board,
                 int last_drawn)
{
    int sum = 0;
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            if (!mark[r][c]) sum += board[r][c];
        }
    }
    return sum * last_drawn;
}

static int run(const vector<int>& draw_list,
               const vector<array<array<int, 5>, 5>>& boards,
               bool last = false)
{
    int left = boards.size();
    vector<bool> won(boards.size());
    vector<array<array<bool, 5>, 5>> marks(boards.size());
    for (int drawn : draw_list) {
        for (size_t i = 0; i < boards.size(); i++) {
            if (won[i]) continue;

            // Mark the board
            const auto& board = boards[i];
            auto& mark = marks[i];
            for (int r = 0; r < 5; r++) {
                for (int c = 0; c < 5; c++) {
                    if (board[r][c] == drawn) {
                        mark[r][c] = true;
                    }
                }
            }

            // Check if the board has bingo
            if (has_won(mark)) {
                if (!last || left == 1) {
                    return score(mark, board, drawn);
                }
                left--;
                won[i] = true;
            }
        }
    }
    return 0;
}

int main()
{
    vector<int> draw_list;
    vector<array<array<int, 5>, 5>> boards;
    string line;
    getline(cin, line);
    stringstream ss(line);
    int v;
    while (ss >> v) {
        draw_list.push_back(v);
        char c;
        ss >> c;
    }
    while (getline(cin, line)) {
        boards.emplace_back();
        auto& b = boards.back();
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                cin >> v;
                b[r][c] = v;
            }
        }
        getline(cin, line);
    }

    cout << "Part 1: " << run(draw_list, boards) << endl;
    cout << "Part 2: " << run(draw_list, boards, true) << endl;

    return 0;
}
