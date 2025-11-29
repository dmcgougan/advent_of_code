/*
 * Problem 21, Advent of Code 2020
 * Danjel McGougan
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main()
{
    // Parse input
    vector<pair<vector<string>, vector<string>>> list;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string word;
        auto& p = list.emplace_back();
        for (;;) {
            assert(ss >> word);
            if (word == "(contains") break;
            p.first.emplace_back(move(word));
        }
        while (ss >> word) {
            p.second.emplace_back(word.substr(0, word.size() - 1));
        }
    }

    // Build sets of ingredients and allergens
    unordered_map<string, unordered_set<string>>
        ing_absent;  // map from ingredient to allergens that can not be in it
    unordered_set<string> ingredients;
    unordered_set<string> allergens;
    for (const auto& [ilist, alist] : list) {
        for (const string& i : ilist) {
            ingredients.insert(i);
            ing_absent[i].clear();
        }
        for (const string& a : alist) allergens.insert(a);
    }

    // Solve part 1
    for (const auto& [ilist, alist] : list) {
        // Check which ingredients are absent from this entry
        unordered_set<string> absent(ingredients);
        for (const string& i : ilist) {
            absent.erase(i);
        }
        // The allergens can not be in those absent ingredients
        for (const string& i : absent) {
            for (const string& a : alist) {
                ing_absent[i].insert(a);
            }
        }
    }
    int part1 = 0;
    vector<string> remove_ing;
    for (const auto& [ing, absent] : ing_absent) {
        if (absent.size() < allergens.size()) continue;
        remove_ing.emplace_back(ing);
        for (const auto& [ilist, alist] : list) {
            for (const string& i : ilist) {
                if (ing == i) ++part1;
            }
        }
    }
    for (const string& i : remove_ing) {
        ing_absent.erase(i);
        ingredients.erase(i);
    }
    remove_ing.clear();

    // Solve part 2
    assert(ingredients.size() == allergens.size());
    vector<pair<string, string>> clist;
    size_t allergen_count = allergens.size();
    for (;;) {
        vector<string> remove_ing;
        vector<string> remove_all;
        bool changed = false;
        for (const auto& [ing, absent] : ing_absent) {
            if (absent.size() + 1 == allergens.size()) {
                // Find which allergen is missing from the absent list
                unordered_set<string> all(allergens);
                for (const string& a : absent) all.erase(a);
                assert(all.size() == 1);
                const string& allergen = *all.begin();
                clist.emplace_back(make_pair(allergen, ing));
                remove_ing.emplace_back(ing);
                remove_all.emplace_back(allergen);
                changed = true;
            }
        }
        if (clist.size() == allergen_count) break;
        assert(changed);
        for (const string& i : remove_ing) {
            ing_absent.erase(i);
            ingredients.erase(i);
        }
        for (const string& a : remove_all) {
            for (auto& [ing, absent] : ing_absent) {
                absent.erase(a);
            }
            allergens.erase(a);
        }
    }
    sort(clist.begin(), clist.end());
    string part2;
    for (size_t i = 0; i < clist.size(); ++i) {
        part2 += clist[i].second;
        if (i + 1 < clist.size()) part2 += ',';
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
