/*
 * Problem 3, Advent of Code 2023
 * Danjel McGougan
 */

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> engine;
int rows;
int cols;

bool issymbol(char c) { return !isdigit(c) && c != '.'; }

int partnum(int row, int col)
{
    int num = 0;
    char c;
    while (col > 0 && isdigit(engine[row][col - 1])) --col;
    while (col < cols && isdigit((c = engine[row][col]))) {
        num = num * 10 + (c - '0');
        ++col;
    }
    return num;
}

int findparts(int row, int col, int& partcount)
{
    int ratio = 1;
    bool d1 = col > 0 && isdigit(engine[row][col - 1]);
    bool d2 = isdigit(engine[row][col]);
    bool d3 = col + 1 < cols && isdigit(engine[row][col + 1]);
    if (d1 && !d2) {
        ratio *= partnum(row, col - 1);
        ++partcount;
    }
    if (d2) {
        ratio *= partnum(row, col);
        ++partcount;
    }
    if (d3 && !d2) {
        ratio *= partnum(row, col + 1);
        ++partcount;
    }
    return ratio;
}

int main()
{
    int part1 = 0;
    int part2 = 0;
    string line;

    while (getline(cin, line)) {
        engine.emplace_back(move(line));
    }
    rows = engine.size();
    cols = engine[0].size();

    for (int row = 0; row < rows; ++row) {
        int num = 0;
        bool adj = false;
        for (int col = 0; col < cols; ++col) {
            char c = engine[row][col];
            // Part 1
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
                if (row > 0 && col > 0 && issymbol(engine[row - 1][col - 1])) adj = true;
                if (row > 0 && issymbol(engine[row - 1][col])) adj = true;
                if (row > 0 && col + 1 < cols && issymbol(engine[row - 1][col + 1])) adj = true;
                if (col + 1 < cols && issymbol(engine[row][col + 1])) adj = true;
                if (row + 1 < rows && col + 1 < cols && issymbol(engine[row + 1][col + 1]))
                    adj = true;
                if (row + 1 < rows && issymbol(engine[row + 1][col])) adj = true;
                if (row + 1 < rows && col > 0 && issymbol(engine[row + 1][col - 1])) adj = true;
                if (col > 0 && issymbol(engine[row][col - 1])) adj = true;
            } else {
                if (adj) part1 += num;
                num = 0;
                adj = false;
            }
            // Part 2
            int ratio = 1;
            int partcount = 0;
            if (c == '*') {
                if (col > 0 && isdigit(engine[row][col - 1])) {
                    ratio *= partnum(row, col - 1);
                    ++partcount;
                }
                if (col + 1 < cols && isdigit(engine[row][col + 1])) {
                    ratio *= partnum(row, col + 1);
                    ++partcount;
                }
                if (row > 0) {
                    ratio *= findparts(row - 1, col, partcount);
                }
                if (row + 1 < rows) {
                    ratio *= findparts(row + 1, col, partcount);
                }
                if (partcount == 2) part2 += ratio;
            }
        }
        if (adj) part1 += num;
        num = 0;
        adj = false;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
