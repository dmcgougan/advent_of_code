/*
 * Problem 13, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <print>
#include <source_location>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned int line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, source_location loc = source_location::current())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(loc.line());
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<string> grid;
    map<array<int, 2>, array<int, 2>> initial_carts;  // [row, col] -> [dir, turn]
    {
        string line;
        for (int row = 0; getline(in, line); ++row) {
            for (int col = 0; col < int(line.size()); ++col) {
                size_t dir;
                if ((dir = string("^>v<").find(line[col])) != string::npos) {
                    line[col] = "|-|-"[dir];
                    initial_carts[{row, col}] = {int(dir), 0};
                }
            }
            grid.emplace_back(std::move(line));
        }
        CHECK(!grid.empty());
    }

    // Common
    auto tick = [&](map<array<int, 2>, array<int, 2>>& carts,
                    bool run_until_one = false) -> optional<string> {
        map<array<int, 2>, array<int, 2>> old_carts = carts;
        carts.clear();
        for (auto it0 = old_carts.begin(); it0 != old_carts.end(); it0 = old_carts.erase(it0)) {
            auto [row, col] = it0->first;
            auto [dir, turn] = it0->second;
            // Move
            static const int D[][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
            row += D[dir][0];
            col += D[dir][1];
            // Check if we crashed
            map<array<int, 2>, array<int, 2>>::iterator it1;
            if ((it1 = carts.find({row, col})) != carts.end()) {
                if (!run_until_one) return to_string(col) + ',' + to_string(row);
                carts.erase(it1);
                continue;
            }
            if ((it1 = old_carts.find({row, col})) != old_carts.end()) {
                if (!run_until_one) return to_string(col) + ',' + to_string(row);
                CHECK(it1 != it0);
                old_carts.erase(it1);
                continue;
            }
            // Static turns
            char path = grid[row][col];
            CHECK(path != ' ');
            if ((dir == 0 || dir == 2) && path == '\\')
                dir = (dir + 3) % 4;
            else if ((dir == 1 || dir == 3) && path == '\\')
                dir = (dir + 1) % 4;
            else if ((dir == 0 || dir == 2) && path == '/')
                dir = (dir + 1) % 4;
            else if ((dir == 1 || dir == 3) && path == '/')
                dir = (dir + 3) % 4;
            // Dynamic turn
            if (path == '+') {
                if (turn == 0) dir = (dir + 3) % 4;  // left
                if (turn == 2) dir = (dir + 1) % 4;  // right
                turn = (turn + 1) % 3;
            }
            carts[{row, col}] = {dir, turn};
        }
        CHECK(!carts.empty());
        if (carts.size() == 1) {
            return to_string(carts.begin()->first[1]) + ',' + to_string(carts.begin()->first[0]);
        }
        return nullopt;
    };

    // Solve part 1
    string part1;
    {
        map<array<int, 2>, array<int, 2>> carts = initial_carts;
        for (;;) {
            auto pos = tick(carts);
            if (pos) {
                part1 = *pos;
                break;
            }
        }
    }

    // Solve part 2
    string part2;
    {
        map<array<int, 2>, array<int, 2>> carts = initial_carts;
        for (;;) {
            auto pos = tick(carts, true);
            if (pos) {
                part2 = *pos;
                break;
            }
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
