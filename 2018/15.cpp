/*
 * Problem 15, Advent of Code 2018
 * Danjel McGougan
 */

#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <set>
#include <source_location>
#include <unordered_map>
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

template <class T, size_t N>
struct array_hash_t
{
    size_t operator()(array<T, N> const& a) const noexcept
    {
        size_t h = 0;
        for (auto const& x : a) {
            h ^= hash<T>{}(x) + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    vector<string> grid;
    for (string line; getline(in, line);) {
        CHECK(grid.empty() || line.size() == grid[0].size());
        grid.emplace_back(std::move(line));
    }
    CHECK(!grid.empty());
    int rows = grid.size();
    int cols = grid[0].size();

    // Identify the units: elves and goblins
    static const int D[][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};  // directions in reading order
    // [row, col] -> [hp, turn_done]
    unordered_map<array<int, 2>, pair<int, bool>, array_hash_t<int, 2>> units;
    int elves = 0;
    int goblins = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'E') {
                units[{r, c}] = {200, false};
                ++elves;
            }
            if (grid[r][c] == 'G') {
                units[{r, c}] = {200, false};
                ++goblins;
            }
        }
    }

    // Save the initial state
    vector<string> init_grid = grid;
    unordered_map<array<int, 2>, pair<int, bool>, array_hash_t<int, 2>> init_units = units;
    int init_elves = elves;
    int init_goblins = goblins;

    // Reset to initial state
    auto reset = [&]() -> void {
        grid = init_grid;
        units = init_units;
        elves = init_elves;
        goblins = init_goblins;
    };

    // Let the unit attack an orhogonally adjacent enemy
    auto attack = [&](int ap, int row, int col, char type) -> bool {
        char enemy = (type == 'E' ? 'G' : 'E');
        int min_hp = INT_MAX;
        int enemy_row = 0;
        int enemy_col = 0;
        for (auto [dr, dc] : D) {
            int rr = row + dr;
            int cc = col + dc;
            if (rr < 0 || rr >= rows || cc < 0 || cc >= cols) continue;
            if (grid[rr][cc] == enemy) {
                auto it = units.find({rr, cc});
                CHECK(it != units.end());
                if (it->second.first < min_hp) {
                    min_hp = it->second.first;
                    enemy_row = rr;
                    enemy_col = cc;
                }
            }
        }
        if (min_hp == INT_MAX) return false;
        // Do the attack
        auto it = units.find({enemy_row, enemy_col});
        CHECK(it != units.end());
        it->second.first -= (type == 'E' ? ap : 3);
        if (it->second.first <= 0) {
            // Enemy dead
            grid[enemy_row][enemy_col] = '.';
            units.erase(it);
            if (enemy == 'E') --elves;
            if (enemy == 'G') --goblins;
        }
        return true;
    };

    // Move one step and then do an attack if possible
    auto move_and_attack = [&](int ap, int row, int col, char type) -> void {
        char enemy = (type == 'E' ? 'G' : 'E');

        // Multi-source BFS searching backwards
        unordered_map<array<int, 2>, pair<int, set<array<int, 2>>>, array_hash_t<int, 2>> info;
        queue<array<int, 2>> q;  // [row, col]
        for (auto [pos, _] : units) {
            auto [r, c] = pos;
            if (grid[r][c] != enemy) continue;
            for (auto [dr, dc] : D) {
                int rr = r + dr;
                int cc = c + dc;
                if (rr < 0 || rr >= rows || cc < 0 || cc >= cols) continue;
                if (grid[rr][cc] == '.') {
                    // Found an empty tile orthogonal to an enemy
                    auto& [dist, owners] = info[{rr, cc}];
                    dist = 0;
                    owners.insert({rr, cc});
                    q.push({rr, cc});
                }
            }
        }
        if (q.empty()) return;
        int min_dist = INT_MAX;
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            auto it = info.find({r, c});
            CHECK(it != info.end());
            const auto& [dist, owners] = it->second;
            if (r == row && c == col) {
                // Found the target node
                min_dist = dist;
                continue;
            }
            if (dist >= min_dist) continue;
            for (auto [dr, dc] : D) {
                int rr = r + dr;
                int cc = c + dc;
                if (rr < 0 || rr >= rows || cc < 0 || cc >= cols) continue;
                if (grid[rr][cc] != '.' && (rr != row || cc != col)) continue;
                auto it = info.find({rr, cc});
                if (it == info.end()) {
                    info.emplace(array{rr, cc}, pair{dist + 1, owners});
                    q.push({rr, cc});
                } else if (it->second.first == dist + 1) {
                    it->second.second.insert(owners.begin(), owners.end());
                }
            }
        }

        // Get the tile adjacent to an enemy that we have a shortest path to
        // and is first in reading order (relies on set and not unordered_set)
        int erow, ecol;
        {
            auto it = info.find({row, col});
            if (it == info.end()) {
                // No path to any enemy
                return;
            }
            erow = (*it->second.second.begin())[0];
            ecol = (*it->second.second.begin())[1];
        }

        // Determine where we should move to reach that enemy using a shortest path
        // Check directions in reading order
        int trow = -1;
        int tcol = -1;
        for (auto [dr, dc] : D) {
            int r = row + dr;
            int c = col + dc;
            auto it = info.find({r, c});
            if (it != info.end() && it->second.first + 1 == min_dist &&
                it->second.second.contains({erow, ecol}))
            {
                // Found it
                trow = r;
                tcol = c;
                break;
            }
        }
        CHECK(trow >= 0);

        // Move the unit
        CHECK(grid[row][col] == type);
        CHECK(grid[trow][tcol] == '.');
        auto it = units.find({row, col});
        CHECK(it != units.end());
        auto hp = it->second;
        units.erase(it);
        units[{trow, tcol}] = hp;
        grid[row][col] = '.';
        grid[trow][tcol] = type;

        // Attack
        attack(ap, trow, tcol, type);
    };

    auto turn = [&](int ap, int row, int col, char type) -> bool {
        // Check if there are any enemies left
        if ((type == 'E' ? !goblins : !elves)) return true;
        // Check if we can attack directly
        if (attack(ap, row, col, type)) {
            return (type == 'E' ? !goblins : !elves);
        }
        // Move first, then attack
        move_and_attack(ap, row, col, type);
        return (type == 'E' ? !goblins : !elves);
    };

    auto round = [&](int ap) -> bool {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                char tile = grid[r][c];
                if (tile != 'E' && tile != 'G') continue;
                auto it = units.find({r, c});
                CHECK(it != units.end());
                if (it->second.second) continue;
                it->second.second = true;
                if (turn(ap, r, c, tile)) return true;
            }
        }
        for (auto& [_, info] : units) info.second = false;
        return false;
    };

    // Solve part 1
    int part1 = 0;
    {
        int rounds = 0;
        while (!round(3)) ++rounds;
        int tot_hp = 0;
        for (auto [_, info] : units) tot_hp += info.first;
        part1 = rounds * tot_hp;
    }

    // Solve part 2
    int part2 = 0;
    {
        auto test = [&](int ap) -> int {
            reset();
            int rounds = 0;
            while (!round(ap)) ++rounds;
            if (elves == init_elves) {
                // No dead elves!
                int tot_hp = 0;
                for (auto [_, info] : units) tot_hp += info.first;
                return rounds * tot_hp;
            }
            return 0;
        };
        int ap = 4;
        while (!(part2 = test(ap))) ++ap;
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
