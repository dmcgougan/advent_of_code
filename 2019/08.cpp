/*
 * Problem 8, Advent of Code 2019
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    const int rows = 6;
    const int cols = 25;
    int layers;
    vector<array<array<int, cols>, rows>> image;
    {
        string line;
        assert(getline(cin, line));
        assert(line.size() % (rows * cols) == 0);
        layers = line.size() / (rows * cols);
        for (int l = 0, i = 0; l < layers; ++l) {
            auto& layer = image.emplace_back();
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    layer[r][c] = line[i++] - '0';
                }
            }
        }
    }

    // Solve part 1
    int part1 = 0;
    int min_zeros = INT_MAX;
    for (int l = 0; l < layers; ++l) {
        const auto& layer = image[l];
        int zeros = 0;
        int ones = 0;
        int twos = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int p = layer[r][c];
                zeros += (p == 0);
                ones += (p == 1);
                twos += (p == 2);
            }
        }
        if (zeros < min_zeros) {
            min_zeros = zeros;
            part1 = ones * twos;
        }
    }
    cout << "Part 1: " << part1 << endl;

    // Solve part 2
    cout << "Part 2:" << endl;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            for (int l = 0; l < layers; ++l) {
                int p = image[l][r][c];
                if (image[l][r][c] != 2) {
                    cout << (p == 1 ? '#' : ' ');
                    break;
                }
            }
        }
        cout << endl;
    }

    return 0;
}
