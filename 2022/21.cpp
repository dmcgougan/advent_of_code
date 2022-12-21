/*
 * Problem 21, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// Split a string into a vector of strings based on the specified separator
static vector<string> split(const string& str, const string& sep)
{
    string s = str;
    vector<string> result;
    string::size_type n;
    assert(!sep.empty());
    while ((n = s.find(sep)) != string::npos) {
        if (n > 0) {
            result.emplace_back(s.substr(0, n));
        }
        s = s.substr(n + sep.size());
    }
    if (!s.empty()) {
        result.emplace_back(move(s));
    }
    return result;
}

struct node
{
    // value
    bool is_value;
    ll value;

    // expression
    string lhs_name;
    string rhs_name;
    node* lhs = nullptr;
    node* rhs = nullptr;
    char op;

    // human (unknown value)
    bool is_human = false;
};

// Resolve the lhs and rhs pointers in all nodes
static void resolve_pointers(map<string, node>& tree)
{
    // Resolve the node pointers
    for (auto& [name, n] : tree) {
        if (!n.is_value) {
            auto lhs = tree.find(n.lhs_name);
            auto rhs = tree.find(n.rhs_name);
            assert(lhs != tree.end() && rhs != tree.end());
            n.lhs = &lhs->second;
            n.rhs = &rhs->second;
        }
    }
}

// Recursively calculate the value of the given tree
static ll calc_tree(node* m)
{
    if (m->is_value) return m->value;
    ll lhs = calc_tree(m->lhs);
    ll rhs = calc_tree(m->rhs);
    switch (m->op) {
    case '*':
        return lhs * rhs;
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '/':
        return lhs / rhs;
    default:
        assert(false);
    }
}

// Check if the given tree contains the human
static bool contains_human(node* tree)
{
    if (tree->is_human) return true;
    if (tree->is_value) return false;
    return contains_human(tree->lhs) || contains_human(tree->rhs);
}

// Solve equation
static ll solve(node* lhs, node* rhs)
{
    for (;;) {
        if (lhs->is_human) return calc_tree(rhs);
        if (rhs->is_human) return calc_tree(lhs);
        if (contains_human(rhs)) {
            swap(lhs, rhs);
        }
        if (contains_human(lhs->lhs)) {
            // Left tree contains the unknown; move the right tree over
            // [H + <number1> = <number2>] -> [H = <number2> - <number1>]
            // [H - <number1> = <number2>] -> [H = <number2> + <number1>]
            // [H * <number1> = <number2>] -> [H = <number2> / <number1>]
            // [H / <number1> = <number2>] -> [H = <number2> * <number1>]
            switch (lhs->op) {
            case '+':
                lhs->op = '-';
                break;
            case '-':
                lhs->op = '+';
                break;
            case '*':
                lhs->op = '/';
                break;
            case '/':
                lhs->op = '*';
                break;
            default:
                assert(false);
            }
            node* n_lhs = lhs->lhs;
            lhs->lhs = rhs;
            rhs = lhs;
            lhs = n_lhs;
        } else {
            assert(contains_human(lhs->rhs));
            // Right tree contains the unknown; move the left tree over
            // [<number1> + H = <number2>] -> [H = <number2> - <number1>]
            // [<number1> - H = <number2>] -> [H = <number1> - <number2>]
            // [<number1> * H = <number2>] -> [H = <number2> / <number1>]
            // [<number1> / H = <number2>] -> [H = <number1> / <number2>]
            bool swap_it = false;
            switch (lhs->op) {
            case '+':
                lhs->op = '-';
                break;
            case '-':
                swap_it = true;
                break;
            case '*':
                lhs->op = '/';
                break;
            case '/':
                swap_it = true;
                break;
            default:
                assert(false);
            }
            node* n_lhs = lhs->rhs;
            if (swap_it) {
                lhs->rhs = rhs;
            } else {
                lhs->rhs = lhs->lhs;
                lhs->lhs = rhs;
            }
            rhs = lhs;
            lhs = n_lhs;
        }
    }
}

int main()
{
    map<string, node> tree;

    // Parse the input
    string line;
    while (getline(cin, line)) {
        auto words = split(line, " ");
        assert(words.size() >= 2);
        node n;
        string name = words[0].substr(0, 4);
        if (words.size() == 2) {
            n.is_value = true;
            stringstream(words[1]) >> n.value;
        } else {
            assert(words.size() == 4);
            n.is_value = false;
            n.lhs_name = words[1];
            n.op = words[2][0];
            n.rhs_name = words[3];
        }
        tree[name] = n;
    }

    // --------------
    // --- Part 1 ---
    // --------------

    map<string, node> p1_tree = tree;
    resolve_pointers(p1_tree);
    auto root_iter = p1_tree.find("root");
    assert(root_iter != p1_tree.end());
    cout << "Part 1: " << calc_tree(&root_iter->second) << endl;

    // --------------
    // --- Part 2 ---
    // --------------

    map<string, node> p2_tree = tree;
    resolve_pointers(p2_tree);

    // Divide into two trees: left of root and right of root
    root_iter = p2_tree.find("root");
    node* root = &root_iter->second;
    node* lhs = root->lhs;
    node* rhs = root->rhs;

    // Find the human and check that exactly one tree contains the human
    auto iter = p2_tree.find("humn");
    assert(iter != p2_tree.end());
    node* human = &iter->second;
    human->is_human = true;
    assert((contains_human(lhs) ? 1 : 0) + (contains_human(rhs) ? 1 : 0) == 1);

    // Solve the equation
    cout << "Part 2: " << solve(lhs, rhs) << endl;

    return 0;
}
