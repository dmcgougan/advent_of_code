/*
 * Problem 23, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

static int calc_cost(const vector<string>& grid)
{
    // #############
    // #01.2.3.4.56#
    // ###0#1#2#3###

    // Room to/from hallway
    // { cost, check_from, check_to }
    static const tuple<int, int, int> r2h[4][7]{
        {{3, 0, 1}, {2, 1, 1}, {2, 2, 2}, {4, 2, 3}, {6, 2, 4}, {8, 2, 5}, {9, 2, 6}},
        {{5, 0, 2}, {4, 1, 2}, {2, 2, 2}, {2, 3, 3}, {4, 3, 4}, {6, 3, 5}, {7, 3, 6}},
        {{7, 0, 3}, {6, 1, 3}, {4, 2, 3}, {2, 3, 3}, {2, 4, 4}, {4, 4, 5}, {5, 4, 6}},
        {{9, 0, 4}, {8, 1, 4}, {6, 2, 4}, {4, 3, 4}, {2, 4, 4}, {2, 5, 5}, {3, 5, 6}}};

    // Room to room
    // { cost, check_from, check_to }
    static const tuple<int, int, int> r2r[4][4]{{{0, 0, 0}, {4, 2, 2}, {6, 2, 3}, {8, 2, 4}},
                                                {{4, 2, 2}, {0, 0, 0}, {4, 3, 3}, {6, 3, 4}},
                                                {{6, 2, 3}, {4, 3, 3}, {0, 0, 0}, {4, 4, 4}},
                                                {{8, 2, 4}, {6, 3, 4}, {4, 4, 4}, {0, 0, 0}}};

    // Info about each amphipod: source room and destination room
    vector<pair<int, int>> podinfo;

    // State for each amphipod: stage and position
    // Stage 0: Still in initial wrong room, position gives where
    // Stage 1: In hallway, position gives where
    // Stage 2: In correct room, not moving anymore
    vector<pair<int, int>> podstate0;

    // Parse the grid
    string line;
    for (size_t i = 0; i < grid.size(); ++i) {
        const string& line = grid[i];
        for (size_t col = 0; col < line.size(); ++col) {
            char ch = line[col];
            if (ch >= 'A' && ch <= 'D') {
                assert(i >= 2 && i <= 5);
                int src = (col - 3) / 2;
                assert(src < 4);
                int dst = ch - 'A';
                podinfo.push_back({src, dst});
                podstate0.push_back({src == dst && i == grid.size() - 2 ? 2 : 0, i - 2});
            }
        }
    }
    int rows = podinfo.size() / 4;

    // Minimum priority queue with { cost, state }
    priority_queue<pair<int, vector<pair<int, int>>>,
                   vector<pair<int, vector<pair<int, int>>>>,
                   greater<pair<int, vector<pair<int, int>>>>>
        queue;

    // Starting state
    queue.push({0, podstate0});

    // Dijkstra
    int result = -1;
    while (!queue.empty()) {
        auto [cost_acc, podstate] = queue.top();
        queue.pop();

        // Build state information
        int room_wrong[4]{};  // how many wrong amphipods does the room have
        int room_right[4]{};  // how many right amphipods does the room have
        bool hall_occupied[7]{};
        bool goal = true;
        for (size_t i = 0; i < podstate.size(); ++i) {
            auto [stage, pos] = podstate[i];
            const auto& [src, dst] = podinfo[i];
            if (stage == 1) {
                assert(!hall_occupied[pos]);
                hall_occupied[pos] = true;
                goal = false;
            } else if (stage == 2) {
                ++room_right[dst];
            } else {
                ++room_wrong[src];
                goal = false;
            }
        }

        // Check if we reached the goal state
        if (goal) {
            result = cost_acc;
            break;
        }

        // Explore possible next moves
        static const int costs[]{1, 10, 100, 1000};
        for (size_t i = 0; i < podstate.size(); ++i) {
            auto& [stage, pos] = podstate[i];
            const auto& [src, dst] = podinfo[i];
            if (stage == 0) {
                // Check if we can move out of the initial room
                if (pos != rows - (room_right[src] + room_wrong[src])) continue;

                // Check if we can move directly into the destination room
                if (!room_wrong[dst]) {
                    const auto& [cost, from, to] = r2r[src][dst];
                    bool free = true;
                    for (int hi = from; hi <= to; ++hi) {
                        if (hall_occupied[hi]) {
                            free = false;
                            break;
                        }
                    }
                    if (free) {
                        stage = 2;
                        queue.push(
                            {cost_acc + (cost + pos + rows - room_right[dst] - 1) * costs[dst],
                             podstate});
                        stage = 0;
                        continue;
                    }
                }

                // Check where we should move to in the hallway
                for (int h = 0; h < 7; ++h) {
                    const auto& [cost, from, to] = r2h[src][h];
                    bool free = true;
                    for (int hi = from; hi <= to; ++hi) {
                        if (hall_occupied[hi]) {
                            free = false;
                            break;
                        }
                    }
                    if (!free) continue;
                    int old_pos = pos;
                    stage = 1;
                    pos = h;
                    queue.push({cost_acc + (cost + old_pos) * costs[dst], podstate});
                    pos = old_pos;
                    stage = 0;
                }
            } else if (stage == 1) {
                // Check if we can move into the correct room
                if (room_wrong[dst]) continue;
                const auto& [cost, from, to] = r2h[dst][pos];
                bool free = true;
                for (int hi = from; hi <= to; ++hi) {
                    if (hi != pos && hall_occupied[hi]) {
                        free = false;
                        break;
                    }
                }
                if (!free) continue;
                stage = 2;
                queue.push({cost_acc + (cost + rows - room_right[dst] - 1) * costs[dst], podstate});
                stage = 1;
            }
        }
    }

    return result;
}

int main()
{
    vector<string> grid;
    string line;
    while (getline(cin, line)) {
        grid.push_back(move(line));
    }

    cout << "Part 1: " << calc_cost(grid) << endl;

    grid.insert(grid.begin() + 3, "  #D#C#B#A#");
    grid.insert(grid.begin() + 4, "  #D#B#A#C#");
    cout << "Part 2: " << calc_cost(grid) << endl;

    return 0;
}
