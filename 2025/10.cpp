/*
 * Problem 10, Advent of Code 2025
 * Danjel McGougan
 *
 * This solution is really slow and takes several hours to run on a fast machine,
 * but it does eventually give the correct answer. I suspect you need a proper
 * ILP solver to do it fast.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, start = 0;
    while ((p = s.find(sep, start)) != string::npos) {
        if (p > start) ret.emplace_back(s.substr(start, p - start));
        start = p + sep.size();
    }
    if (start < s.size()) ret.emplace_back(s.substr(start));
    return ret;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<tuple<int, vector<int>, vector<int>>> machines;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " ");
        assert(s.size() >= 3);
        auto& [lights, buttons, joltages] = machines.emplace_back();
        for (size_t i = 1, b = 1; i + 1 < s[0].size(); ++i, b <<= 1) {
            if (s[0][i] == '#') lights |= b;
        }
        for (size_t i = 1; i + 1 < s.size(); ++i) {
            stringstream ss(s[i]);
            int& button = buttons.emplace_back();
            while (ss && ss.peek() != ')') {
                ss.ignore(1);
                int bbit;
                ss >> bbit;
                button |= (1 << bbit);
            }
        }
        stringstream ss(s[s.size() - 1]);
        while (ss && ss.peek() != '}') {
            ss.ignore(1);
            ss >> joltages.emplace_back();
        }
        std::sort(buttons.begin(), buttons.end(), [](int a, int b) {
            return __builtin_popcount(a) > __builtin_popcount(b);
        });
    }
    assert(!machines.empty());

    // Solve part 1
    int part1 = 0;
    {
        for (const auto& [lights, buttons, _] : machines) {
            // BFS
            queue<array<int, 2>> q;
            unordered_set<int> seen;
            q.push({0, 0});
            seen.insert(0);
            int min_count = -1;
            while (!q.empty()) {
                auto [l, count] = q.front();
                q.pop();
                if (l == lights) {
                    min_count = count;
                    break;
                }
                for (int b : buttons) {
                    int next = l ^ b;
                    if (seen.count(next)) continue;
                    q.push({next, count + 1});
                    seen.insert(next);
                }
            }
            assert(min_count >= 0);
            part1 += min_count;
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        for (const auto& m : machines) {
            const vector<int>& buttons = get<1>(m);
            int bsize = buttons.size();
            const vector<int>& joltages = get<2>(m);
            int jsize = joltages.size();

            // Calculate a simple upper bound for pruning
            int best = 0;
            for (int j : joltages) best += j;
            int min_cov = INT_MAX;
            for (int mask : buttons) {
                min_cov = min(min_cov, __builtin_popcount(mask));
            }
            assert(min_cov != INT_MAX && min_cov >= 1);
            best = (best + min_cov - 1) / min_cov;

            // Remaining increments until a counter reaches its goal
            vector<int> rem = joltages;

            auto dfs = [&](auto self, int button, int count) -> void {
                if (count >= best) return;

                // Compute mask of counters still above zero and some summary info
                int rem_mask = 0;
                int rem_sum = 0;
                int lb1 = 0;
                for (int i = 0; i < jsize; ++i) {
                    int r = rem[i];
                    if (r > 0) {
                        rem_mask |= (1 << i);
                        rem_sum += r;
                        lb1 = max(lb1, r);
                    }
                }

                // Goal test: everything is zero
                if (rem_mask == 0) {
                    best = min(best, count);
                    return;
                }
                if (button == bsize) return;

                // Compute "max coverage" of any remaining button on currently positive counters
                int max_cov_rem = 0;
                for (int bi = button; bi < bsize; ++bi) {
                    int cov = __builtin_popcount(buttons[bi] & rem_mask);
                    if (cov > max_cov_rem) max_cov_rem = cov;
                }
                if (max_cov_rem == 0) return;

                // Lower bound on remaining presses from this state even with perfect choices.
                int lb2 = (rem_sum + max_cov_rem - 1) / max_cov_rem;
                int node_lb = max(lb1, lb2);

                // Light-weight refinement: only near the top and when there's a lot left to do.
                if (button < 8) {
                    // find two largest remaining counters
                    int max1 = -1, idx1 = -1;
                    int max2 = -1, idx2 = -1;
                    for (int i = 0; i < jsize; ++i) {
                        int r = rem[i];
                        if (r <= 0) continue;
                        if (r > max1) {
                            max2 = max1;
                            idx2 = idx1;
                            max1 = r;
                            idx1 = i;
                        } else if (r > max2) {
                            max2 = r;
                            idx2 = i;
                        }
                    }

                    if (idx1 >= 0 && idx2 >= 0) {
                        int S_mask = (1 << idx1) | (1 << idx2);
                        int sumS = rem[idx1] + rem[idx2];

                        int maxCovS = 0;
                        for (int bi = button; bi < bsize; ++bi) {
                            int cov = __builtin_popcount(buttons[bi] & S_mask);  // 0..2
                            if (cov > maxCovS) maxCovS = cov;
                        }
                        if (maxCovS == 0) {
                            return;
                        }
                        int lbS = (sumS + maxCovS - 1) / maxCovS;
                        if (lbS > node_lb) node_lb = lbS;
                    }
                }
                if (count + node_lb >= best) {
                    return;
                }
                int button_mask = buttons[button];

                // Try skipping this button (zero presses)
                // Check if there are counters that can not reach its goal with the buttons
                // remaining
                bool dead = false;
                for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                    if (rem[i] > 0) {
                        // The counter is not at its goal yet
                        bool reachable = false;
                        for (int bi = button + 1; bi < bsize; ++bi) {
                            if (buttons[bi] & b) {
                                // Found a remaining button that increases the counter
                                reachable = true;
                                break;
                            }
                        }
                        if (!reachable) {
                            dead = true;
                            break;
                        }
                    }
                }
                if (!dead) self(self, button + 1, count);

                // Get max times we can press the button without overflowing any counter
                int presses = INT_MAX;
                for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                    if (button_mask & b) presses = min(presses, rem[i]);
                }
                // If presses makes us go past the current best, then reduce it
                int max_useful = best - count - 1;
                if (max_useful <= 0) return;
                presses = min(presses, max_useful);
                if (presses <= 0) return;

                // Try press counts for the button
                for (int p = presses; p >= 1; --p) {
                    // Prune
                    int lb1 = 0;
                    int rem_sum = 0;
                    int rem_mask2 = 0;
                    for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                        int r = rem[i] - ((button_mask & b) ? p : 0);
                        assert(r >= 0);
                        if (r > 0) rem_mask2 |= b;
                        lb1 = max(lb1, r);
                        rem_sum += r;
                    }
                    if (rem_mask2 == 0) {
                        if (count + p < best) {
                            best = count + p;
                        }
                        continue;
                    }

                    // Compute max coverage of remaining buttons
                    int max_cov_rem2 = 0;
                    for (int bi = button + 1; bi < bsize; ++bi) {
                        int cov = __builtin_popcount(buttons[bi] & rem_mask2);
                        if (cov > max_cov_rem2) max_cov_rem2 = cov;
                    }
                    if (max_cov_rem2 == 0) continue;
                    int lb2 = (rem_sum + max_cov_rem2 - 1) / max_cov_rem2;
                    if (count + p + max(lb1, lb2) >= best) continue;

                    // Check if there are counters that can not reach its goal with the buttons
                    // remaining
                    bool dead = false;
                    for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                        if (rem[i] - ((button_mask & b) ? p : 0) > 0) {
                            // The counter is not at its goal yet
                            bool reachable = false;
                            for (int bi = button + 1; bi < bsize; ++bi) {
                                if (buttons[bi] & b) {
                                    // Found a remaining button that increases the counter
                                    reachable = true;
                                    break;
                                }
                            }
                            if (!reachable) {
                                dead = true;
                                break;
                            }
                        }
                    }
                    if (dead) continue;

                    // Continue search
                    for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                        if (button_mask & b) rem[i] -= p;
                    }
                    self(self, button + 1, count + p);
                    for (int i = 0, b = 1; i < jsize; ++i, b <<= 1) {
                        if (button_mask & b) rem[i] += p;
                    }
                }
            };
            dfs(dfs, 0, 0);
            cout << '.' << flush;
            part2 += best;
        }
        cout << endl;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
