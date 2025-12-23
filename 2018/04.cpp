/*
 * Problem 4, Advent of Code 2018
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 5>> data;
    for (string line; getline(in, line);) {
        int month, day, hour, minute, id;
        if (line.find("begins shift") != string::npos) {
            assert(sscanf(line.c_str(),
                          "[1518-%d-%d %d:%d] Guard #%d begins shift",
                          &month,
                          &day,
                          &hour,
                          &minute,
                          &id) == 5);
        } else {
            assert(sscanf(line.c_str(), "[1518-%d-%d %d:%d]", &month, &day, &hour, &minute) == 4);
            if (line.find("falls asleep") != string::npos) {
                id = -1;
            } else {
                assert(line.find("wakes up") != string::npos);
                id = -2;
            }
        }
        data.push_back({month, day, hour, minute, id});
    }
    assert(!data.empty());

    // Common sleep data
    std::sort(data.begin(), data.end());
    unordered_map<int, array<int, 60>> sleep_data;
    {
        int guard = 0;
        int start_sleep = -1;
        for (auto [month, day, hour, minute, id] : data) {
            if (id >= 0) {
                guard = id;
                assert(start_sleep < 0);
                continue;
            }
            assert(guard);
            if (id == -1) {
                // Falls asleep
                assert(start_sleep < 0);
                start_sleep = minute;
                continue;
            }
            assert(id == -2);
            // Wakes up
            assert(start_sleep >= 0);
            for (int m = start_sleep; m < minute; ++m) {
                assert(m < 60);
                ++sleep_data[guard][m];
            }
            start_sleep = -1;
        }
    }

    // Solve part 1
    int part1 = 0;
    {
        // Check which guard slept the most
        int max_sleep = INT_MIN;
        int max_guard = 0;
        for (const auto& [g, hour] : sleep_data) {
            int sleep = std::accumulate(hour.begin(), hour.end(), 0);
            if (sleep > max_sleep) {
                max_sleep = sleep;
                max_guard = g;
            }
        }
        // Check which minute that guard slept the most
        const auto& hour = sleep_data[max_guard];
        int minute = std::distance(hour.begin(), std::max_element(hour.begin(), hour.end()));
        part1 = max_guard * minute;
    }

    // Solve part 2
    int part2 = 0;
    {
        // Check which guard slept the most on the same minute
        int max_sleep = INT_MIN;
        int max_guard = 0;
        int max_minute = 0;
        for (const auto& [g, hour] : sleep_data) {
            for (int m = 0; m < 60; ++m) {
                if (hour[m] > max_sleep) {
                    max_sleep = hour[m];
                    max_guard = g;
                    max_minute = m;
                }
            }
        }
        part2 = max_guard * max_minute;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
