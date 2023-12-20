/*
 * Problem 20, Advent of Code 2023
 * Danjel McGougan
 */

#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

static map<string, pair<char, vector<string>>> config;
static map<string, int> ff_state;
static map<string, map<string, int>> conj_state;

static ll low_pulses;
static ll high_pulses;

static void clear_state()
{
    low_pulses = 0;
    high_pulses = 0;
    for (const auto& [module, info] : config) {
        const auto& [type, dst_list] = info;
        if (type == '%') {
            ff_state[module] = 0;
        }
        for (const string& dst : dst_list) {
            auto iter = config.find(dst);
            if (iter == config.end()) continue;
            auto& [type, dst_list] = iter->second;
            if (type == '&') {
                conj_state[dst][module] = 0;
            }
        }
    }
}

static bool press_button(string check_module = "")
{
    bool check_module_high = false;
    deque<tuple<string, string, int>> pulse_q;
    string b = "broadcaster";
    auto iter = config.find(b);
    assert(iter != config.end());
    ++low_pulses;  // button
    for (string dst : iter->second.second) {
        ++low_pulses;
        pulse_q.push_back(make_tuple(b, dst, 0));
    }
    while (!pulse_q.empty()) {
        auto [origin, module, pulse] = pulse_q.front();
        pulse_q.pop_front();
        auto iter = config.find(module);
        if (iter == config.end()) continue;
        const auto& [type, dst_list] = iter->second;
        if (type == '%') {
            // Flip-flop
            auto iter = ff_state.find(module);
            assert(iter != ff_state.end());
            int& state = iter->second;
            if (pulse == 0) {
                state ^= 1;
                for (string dst : dst_list) {
                    if (state)
                        ++high_pulses;
                    else
                        ++low_pulses;
                    pulse_q.push_back(make_tuple(module, dst, state));
                }
            }
        } else if (type == '&') {
            // Conjunction
            auto iter = conj_state.find(module);
            assert(iter != conj_state.end());
            auto& state = iter->second;
            state[origin] = pulse;
            int output = 1;
            if (pulse) {
                output = 0;
                for (const auto& [name, input] : state) {
                    if (!input) {
                        output = 1;
                        break;
                    }
                }
            }
            if (module == check_module && output) check_module_high = true;
            for (string dst : dst_list) {
                if (output)
                    ++high_pulses;
                else
                    ++low_pulses;
                pulse_q.push_back(make_tuple(module, dst, output));
            }
        }
    }
    return check_module_high;
}

int main()
{
    // Parse input data
    string rx_input;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string from;
        ss >> from;
        char type = from[0];
        if (type != 'b') from = from.substr(1);
        string to;
        vector<string> dst_list;
        ss >> to;
        while (ss >> to) {
            if (to[to.size() - 1] == ',') to = to.substr(0, to.size() - 1);
            if (to == "rx") rx_input = from;
            dst_list.push_back(to);
        }
        config.insert(make_pair(from, make_pair(type, dst_list)));
    }

    // Part 1
    clear_state();
    for (int i = 0; i < 1000; ++i) {
        press_button();
    }
    ll part1 = low_pulses * high_pulses;

    // Part 2
    // ------
    // rx_input is the conjunction module that feeds 'rx'.
    // For each module that feeds rx_input check its period for outputting a high pulse,
    // then calculate the LCM (least common multiple) of those periods.
    // This is not a general solution by far since the phases of the high pulses might
    // not match in the general case, also the high pulses might not coincide within
    // one button press. It works for my input data and I suspect the data has been
    // prepared to make this a special case that is easier to calculate.
    ll part2 = 1;
    const auto& inputs = conj_state[rx_input];
    for (const auto& [module, state] : inputs) {
        assert(config[module].first == '&');
        clear_state();
        ll period = 0;
        for (;;) {
            ++period;
            if (press_button(module)) break;
        }
        part2 = lcm(part2, period);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
