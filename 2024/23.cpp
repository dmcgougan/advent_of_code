/*
 * Problem 23, Advent of Code 2024
 * Danjel McGougan
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

//
// https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
//
// algorithm BronKerbosch1(R, P, X) is
//     if P and X are both empty then
//         report R as a maximal clique
//     for each vertex v in P do
//         BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
//         P := P \ {v}
//         X := X ⋃ {v}
//

static map<string, set<string>> adj_set;
static map<string, vector<string>> adj_vector;
static set<string> max_set;

static void bron_kerbosch(set<string> R, set<string> P, set<string> X)
{
    if (P.empty() && X.empty()) {
        if (R.size() > max_set.size()) max_set = R;
    }
    for (auto iter = P.begin(); iter != P.end(); iter = P.erase(iter)) {
        const string& v = *iter;
        const set<string>& n_v = adj_set[v];
        set<string> p_n;
        set_intersection(P.begin(), P.end(), n_v.begin(), n_v.end(), inserter(p_n, p_n.begin()));
        set<string> x_n;
        set_intersection(X.begin(), X.end(), n_v.begin(), n_v.end(), inserter(x_n, x_n.begin()));
        set<string> r_v(R);
        r_v.insert(v);
        bron_kerbosch(r_v, p_n, x_n);
        X.insert(v);
    }
}

int main()
{
    set<string> V;
    string line;
    while (getline(cin, line)) {
        string c0 = line.substr(0, 2);
        string c1 = line.substr(3);
        adj_set[c0].emplace(c1);
        adj_set[c1].emplace(c0);
        adj_vector[c0].emplace_back(c1);
        adj_vector[c1].emplace_back(c0);
        V.insert(c0);
        V.insert(c1);
    }

    // Count number of fully connected subsets of size three with a computer starting with 't'
    set<string> triplets;
    for (const auto& [c0, neighbours] : adj_vector) {
        if (c0[0] != 't') continue;
        size_t n = neighbours.size();
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                const string& c1 = neighbours[i];
                const string& c2 = neighbours[j];
                if (adj_set[c1].count(c2)) {
                    // Silly way of removing duplicates
                    // Sort the three computers and insert the resulting triplet into a set
                    vector<string> t;
                    t.emplace_back(c0);
                    t.emplace_back(c1);
                    t.emplace_back(c2);
                    sort(t.begin(), t.end());
                    triplets.insert(t[0] + t[1] + t[2]);
                }
            }
        }
    }

    // Find maximum size fully connected subset
    bron_kerbosch(set<string>(), V, set<string>());
    string part2;
    for (const string& v : max_set) {
        if (!part2.empty()) part2 += ',';
        part2 += v;
    }

    cout << "Part 1: " << triplets.size() << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
