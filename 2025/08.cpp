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

// Disjoint Set Union
// https://cp-algorithms.com/data_structures/disjoint_set_union.html
class dsu_t
{
public:
    explicit dsu_t(int n) : parent(n), sz(n, 1), sets(n)
    {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Get the representative (root) of the set that x belongs to
    int find(int x)
    {
        if (x == parent[x]) return x;
        // Flatten the tree while searching
        return (parent[x] = find(parent[x]));
    }

    // Merge sets a and b
    bool merge(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) std::swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        --sets;
        return true;
    }

    // Get the size of the set that x belongs to
    int size(int x) { return sz[find(x)]; }

    // Get the number of disjoint sets
    int count() const { return sets; }

private:
    vector<int> parent;
    vector<int> sz;
    int sets;
};

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
    using pq_t = tuple<ll, int, int>;
    priority_queue<pq_t, vector<pq_t>, greater<pq_t>> dist;
    dsu_t dsu(boxes);
    {
        // Calculate the square of the distance between all pairs of boxes
        for (int i = 0; i < boxes; ++i) {
            for (int j = i + 1; j < boxes; ++j) {
                ll d0 = box[i][0] - box[j][0];
                ll d1 = box[i][1] - box[j][1];
                ll d2 = box[i][2] - box[j][2];
                // Put the distances in a heap
                dist.push({d0 * d0 + d1 * d1 + d2 * d2, i, j});
            }
        }

        // Merge sets connected by the first 1000 shortest distance pairs
        assert(dist.size() >= 10);
        int pairs = dist.size() >= 1000 ? 1000 : 10;  // also make it work for the test input
        for (int p = 0; p < pairs; ++p) {
            auto [_, a, b] = dist.top();
            dist.pop();
            dsu.merge(a, b);
        }
    }

    // Solve part 1
    ll part1;
    {
        // Get the circuit sizes
        priority_queue<int> circuit_sizes;
        for (int i = 0; i < boxes; ++i) {
            if (dsu.find(i) == i) circuit_sizes.push(dsu.size(i));
        }

        // Compute the answer
        assert(circuit_sizes.size() >= 3);
        part1 = 1;
        for (int i = 0; i < 3; ++i) {
            part1 *= circuit_sizes.top();
            circuit_sizes.pop();
        }
    }

    // Solve part 2
    ll part2 = 0;
    for (;;) {
        // Get next pair
        assert(!dist.empty());
        auto [_, a, b] = dist.top();
        dist.pop();

        // Merge the sets
        dsu.merge(a, b);

        // Check if we are done
        if (dsu.count() == 1) {
            // Only one circuit left so we are fully connected
            part2 = ll(box[a][0]) * box[b][0];
            break;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
