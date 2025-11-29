/*
 * Problem 18, Advent of Code 2021
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Represents a snailfish number
struct sfnum
{
    sfnum() : parent(nullptr), left(nullptr), right(nullptr), left_num(0), right_num(0) { }
    sfnum(const sfnum& n) :
        parent(nullptr), left(nullptr), right(nullptr), left_num(n.left_num), right_num(n.right_num)
    {
        if (n.left) {
            left = new sfnum(*n.left);
            left->parent = this;
        }
        if (n.right) {
            right = new sfnum(*n.right);
            right->parent = this;
        }
    }
    ~sfnum()
    {
        if (left) delete left;
        if (right) delete right;
    }
    sfnum* parent;
    sfnum* left;
    sfnum* right;
    int left_num;
    int right_num;

private:
    // Assignment operator not implemented
    sfnum& operator=(const sfnum& rhs);
};

// Find a node at depth 4 and explode it; returns true iff a node was exploded
// Use iterative walk instead of recursive to make it easier to find the numbers to modify
static bool explode(sfnum* num)
{
    // { node, state (0=left, 1=right), depth }
    vector<tuple<sfnum*, int, int>> q;
    sfnum* exploded_node = nullptr;
    int* num_before = nullptr;
    q.push_back({num, 0, 0});
    while (!q.empty()) {
        auto [node, state, depth] = q.back();
        q.pop_back();
        if (!exploded_node && depth == 4) {
            // Found the node to explode
            assert(!node->left);
            assert(!node->right);
            exploded_node = node;
            continue;
        }
        if (state == 0) {
            q.push_back({node, 1, depth});
            if (node->left) {
                q.push_back({node->left, 0, depth + 1});
            } else if (!exploded_node) {
                num_before = &node->left_num;
            } else {
                // Add to the first number after the exploded node
                node->left_num += exploded_node->right_num;
                break;
            }
            continue;
        }
        if (node->right) {
            q.push_back({node->right, 0, depth + 1});
        } else if (!exploded_node) {
            num_before = &node->right_num;
        } else {
            // Add to the first number after the exploded node
            node->right_num += exploded_node->right_num;
            break;
        }
    }
    if (!exploded_node) return false;

    // Add to the last number before the exploded node
    if (num_before) *num_before += exploded_node->left_num;

    // Replace the exploded node with zero
    if (exploded_node->parent->left == exploded_node) {
        exploded_node->parent->left = nullptr;
        exploded_node->parent->left_num = 0;
    } else {
        assert(exploded_node->parent->right == exploded_node);
        exploded_node->parent->right = nullptr;
        exploded_node->parent->right_num = 0;
    }
    delete exploded_node;

    return true;
}

// Find a node that needs splitting; returns true iff a node was split
static bool split(sfnum* node)
{
    if (node->left) {
        if (split(node->left)) return true;
    } else if (node->left_num > 9) {
        node->left = new sfnum;
        node->left->parent = node;
        node->left->left_num = node->left_num / 2;
        node->left->right_num = (node->left_num + 1) / 2;
        node->left_num = 0;
        return true;
    }
    if (node->right) {
        if (split(node->right)) return true;
    } else if (node->right_num > 9) {
        node->right = new sfnum;
        node->right->parent = node;
        node->right->left_num = node->right_num / 2;
        node->right->right_num = (node->right_num + 1) / 2;
        node->right_num = 0;
        return true;
    }
    return false;
}

// Reduce a snailfish number
static void reduce(sfnum* num)
{
    for (;;) {
        if (explode(num)) {
            continue;
        }
        if (split(num)) {
            continue;
        }
        break;
    }
}

// Add two snailfish numbers together
static sfnum* add(sfnum* arg1, sfnum* arg2)
{
    sfnum* result = new sfnum;
    arg1->parent = result;
    arg2->parent = result;
    result->left = arg1;
    result->right = arg2;
    reduce(result);
    return result;
}

// Parse input and construct the corresponding snailfish number
static sfnum* parse(istream& is, sfnum* parent = nullptr)
{
    sfnum* num = new sfnum;
    num->parent = parent;
    char c;
    is >> c;
    assert(c == '[');
    if (is.peek() == '[') {
        num->left = parse(is, num);
    } else {
        int digit;
        is >> digit;
        num->left_num = digit;
    }
    is >> c;
    assert(c == ',');
    if (is.peek() == '[') {
        num->right = parse(is, num);
    } else {
        int digit;
        is >> digit;
        num->right_num = digit;
    }
    is >> c;
    assert(c == ']');
    return num;
}

// Get the magnitude of a snailfish number
static int magnitude(sfnum* num)
{
    return 3 * (num->left ? magnitude(num->left) : num->left_num) +
        2 * (num->right ? magnitude(num->right) : num->right_num);
}

int main()
{
    string line;
    sfnum* result = nullptr;
    vector<sfnum> nums;
    while (getline(cin, line)) {
        stringstream ss(line);
        sfnum* num = parse(ss);
        nums.emplace_back(*num);
        result = result ? add(result, num) : num;
    }
    int part1 = magnitude(result);
    delete result;

    int part2 = 0;
    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = 0; j < nums.size(); ++j) {
            if (i == j) continue;
            sfnum* sum = add(new sfnum(nums[i]), new sfnum(nums[j]));
            part2 = max(part2, magnitude(sum));
            delete sum;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
