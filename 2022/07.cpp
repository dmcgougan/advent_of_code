/*
 * Problem 7, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using ll = int64_t;

struct directory {
    directory* parent;
    map<string, ll> files;
    map<string, directory> dirs;
};

static ll need_to_free;
static ll p1_size = 0;
static ll p2_size = 0;

static ll
sumDirs(directory* dir)
{
    ll size = 0;
    for (auto& d : dir->dirs) {
        size += sumDirs(&d.second);
    }
    for (const auto& f : dir->files) {
        size += f.second;
    }
    if (size <= 100000) {
        p1_size += size;
    }
    if (size >= need_to_free && size < p2_size) {
        p2_size = size;
    }
    return size;
}

int main()
{
    string line;
    directory root { nullptr };
    directory* cwd = &root;

    // Parse input and populate the directory tree
    while (getline(cin, line)) {
        if (line[0] == '$') {
            // Check for cd
            if (line[2] == 'c') {
                string name = line.substr(5);
                if (name == "..") {
                    // Go up one level
                    if (cwd->parent) {
                        cwd = cwd->parent;
                    }
                } else if (name == "/") {
                    // Go to the root directory
                    cwd = &root;
                } else {
                    // Go down one level
                    auto iter = cwd->dirs.find(name);
                    assert(
                        iter != cwd->dirs.end()); // assume we never cd into an unseen directory
                    cwd = &(*iter).second;
                }
            }
        } else {
            // Directory listing
            if (isdigit((unsigned char)line[0])) {
                // This is a file
                char* end;
                ll size = strtoul(line.c_str(), &end, 10);
                assert(*end == ' ');
                string name = line.substr(end - line.c_str() + 1);
                auto iter = cwd->files.find(name);
                if (iter == cwd->files.end()) {
                    // New file
                    cwd->files[name] = size;
                }
            } else {
                // This is a directory
                string name = line.substr(4);
                auto iter = cwd->dirs.find(name);
                if (iter == cwd->dirs.end()) {
                    // New directory
                    cwd->dirs[name] = directory { cwd };
                }
            }
        }
    }

    ll free_space = 70000000 - sumDirs(&root);
    need_to_free = 30000000 - free_space;

    p1_size = 0;
    p2_size = INT64_MAX;
    sumDirs(&root);

    cout << "Part 1: " << p1_size << endl;
    cout << "Part 2: " << p2_size << endl;

    return 0;
}
