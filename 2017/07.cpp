/*
 * Problem 7, Advent of Code 2017
 * Danjel McGougan
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <print>
#include <source_location>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

[[noreturn]] static void check_failed(unsigned line)
{
    println("CHECK failed at line {}", line);
    exit(1);
}

template <typename T>
static inline void CHECK(T&& ok, unsigned line = source_location::current().line())
{
    if (!static_cast<bool>(ok)) [[unlikely]] {
        check_failed(line);
    }
}

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& s, const string& sep)
{
    vector<string> ret;
    size_t p, p0 = 0;
    while ((p = s.find(sep, p0)) != string::npos) {
        if (p > p0) ret.emplace_back(s.substr(p0, p - p0));
        p0 = p + sep.size();
    }
    if (p0 < s.size()) ret.emplace_back(s.substr(p0));
    return ret;
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    unordered_map<string, pair<int, vector<string>>> nodes;
    for (string line; getline(in, line);) {
        vector<string> s = split(line, " ");
        CHECK(s.size() >= 2);
        CHECK(s[1].size() > 1);
        auto& [weight, adj] = nodes[s[0]];
        weight = atoi(s[1].c_str() + 1);
        if (s.size() > 3) {
            CHECK(s[2] == "->");
            for (size_t i = 3; i < s.size(); ++i) {
                int slen = s[i].size();
                if (slen > 0 && s[i][slen - 1] == ',') --slen;
                adj.emplace_back(s[i].substr(0, slen));
            }
        }
    }

    string part1;
    {
        // Find the node that is not in any adjacency list
        unordered_set<string> n;
        for (const auto& [name, _] : nodes) n.insert(name);
        for (const auto& [_, info] : nodes) {
            for (const string& s : info.second) n.erase(s);
        }
        CHECK(n.size() == 1);
        part1 = *n.begin();
    }

    int part2 = 0;
    {
        auto get_node = [&](const string& node) -> const pair<int, vector<string>>& {
            auto it = nodes.find(node);
            CHECK(it != nodes.end());
            return it->second;
        };
        auto get_weight = [&](const auto& self, const string& node) -> int {
            const auto& [weight, adj] = get_node(node);
            int wsum = weight;
            for (const string& child : adj) {
                wsum += self(self, child);
            }
            return wsum;
        };
        auto is_balanced = [&](const auto& self, const string& node) -> bool {
            int child_weight = -1;
            for (const string& child : get_node(node).second) {
                if (!self(self, child)) return false;
                int weight = get_weight(get_weight, child);
                if (child_weight < 0) {
                    child_weight = weight;
                } else if (weight != child_weight) {
                    return false;
                }
            }
            return true;
        };
        // Start at the root and follow the unbalanced children down
        const string* cur = &part1;
        CHECK(!is_balanced(is_balanced, *cur));
        for (;;) {
            const string* unbalanced_child = nullptr;
            for (const string& child : get_node(*cur).second) {
                if (is_balanced(is_balanced, child)) continue;
                CHECK(!unbalanced_child);
                unbalanced_child = &child;
            }
            if (!unbalanced_child) break;
            cur = unbalanced_child;
        }
        // Found the first node where all children are balanced but the node itself is not balanced
        // Check the weight of the children and find the odd one out
        const auto& [node_w, adj] = get_node(*cur);
        unordered_map<int, int> hist;
        for (const string& child : adj) {
            ++hist[get_weight(get_weight, child)];
        }
        int minority_weight = -1;
        int majority_weight = -1;
        for (auto [w, count] : hist) {
            if (count == 1) {
                CHECK(minority_weight < 0);
                minority_weight = w;
            } else if (count > 1) {
                CHECK(majority_weight < 0);
                majority_weight = w;
            }
        }
        CHECK(minority_weight >= 0 && majority_weight >= 0);
        // Find the child with the odd weight and adjust it
        for (const string& child : adj) {
            if (get_weight(get_weight, child) != minority_weight) continue;
            int& w = nodes[child].first;
            int diff = majority_weight - minority_weight;
            w += diff;
            CHECK(is_balanced(is_balanced, part1));
            part2 = w;
            break;
        }
    }

    // Output answers
    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}
