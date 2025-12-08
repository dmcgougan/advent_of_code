/*
 * Problem 8, Advent of Code 2025
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;
using ll = long long;

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<array<int, 3>> box;
    for (string line; getline(in, line);) {
        stringstream ss(line);
        auto& c = box.emplace_back();
        ss >> c[0];
        ss.ignore(1);
        ss >> c[1];
        ss.ignore(1);
        ss >> c[2];
    }
    assert(!box.empty());
    int boxes = box.size();

    // Common

    // Calculate the square of the distance between all pairs of boxes
    vector<tuple<ll, int, int>> dist;
    for (int i = 0; i < boxes; ++i) {
        for (int j = i + 1; j < boxes; ++j) {
            ll a = box[i][0] - box[j][0];
            ll b = box[i][1] - box[j][1];
            ll c = box[i][2] - box[j][2];
            ll d = a * a + b * b + c * c;
            dist.push_back({d, i, j});
        }
    }

    // Sort the distances in ascending order and build adjacency lists
    // containing the 1000 pairs with shortest distance between them
    std::sort(dist.begin(), dist.end());
    vector<vector<int>> adj(boxes);
    for (int i = 0; i < 1000; ++i) {
        auto [_, a, b] = dist[i];
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // Solve part 1
    int part1;
    {
        // BFS to find connected islands
        vector<bool> visited(boxes);
        vector<int> sizes;
        for (int i = 0; i < boxes; ++i) {
            if (visited[i]) continue;
            int size = 1;
            queue<int> q;
            q.push(i);
            visited[i] = true;
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int next : adj[node]) {
                    if (visited[next]) continue;
                    visited[next] = true;
                    ++size;
                    q.push(next);
                }
            }
            sizes.push_back(size);
        }

        // Sort the island sizes in descending order and compute answer
        std::sort(sizes.rbegin(), sizes.rend());
        assert(sizes.size() >= 3);
        part1 = sizes[0] * sizes[1] * sizes[2];
    }

    // Solve part 2
    ll part2 = 0;
    {
        queue<int> q;
        for (int p = 1000;; ++p) {
            // Add another pair to the adjacency lists
            auto [_, a, b] = dist[p];
            adj[a].push_back(b);
            adj[b].push_back(a);

            // Use BFS to check if the new graph is fully connected
            vector<bool> visited(boxes);
            int size = 1;
            visited[0] = true;
            q.push(0);
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int next : adj[node]) {
                    if (visited[next]) continue;
                    visited[next] = true;
                    ++size;
                    q.push(next);
                }
            }
            if (size == boxes) {
                // Fully connected; compute answer
                part2 = ll(box[a][0]) * box[b][0];
                break;
            }
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
