/*
 * Problem 9, Advent of Code 2024
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;
using ll = int64_t;

// Move blocks
static vector<int> compact_1(const vector<int>& bmap)
{
    vector<int> blockmap(bmap);
    int first = 0;
    int last = blockmap.size() - 1;
    for (;;) {
        while (last > 0 && blockmap[last] < 0) last--;
        while (first < last && blockmap[first] >= 0) first++;
        if (first < last) {
            blockmap[first] = blockmap[last];
            blockmap[last] = -1;
        } else {
            break;
        }
    }
    return blockmap;
}

// Move whole files
static vector<int> compact_2(const vector<int>& bmap,
                             const vector<pair<int, int>>& frmap,
                             const vector<pair<int, int>>& filemap)
{
    vector<int> blockmap(bmap);
    vector<pair<int, int>> freemap(frmap);
    // Iterate over all files in reverse order
    for (int i = filemap.size() - 1; i >= 0; i--) {
        // Find a large enough free space to the left of the file
        int filesize = filemap[i].second;
        for (int j = 0; freemap[j].first < filemap[i].first; j++) {
            if (filesize <= freemap[j].second) {
                // Found space; move the file
                int from = filemap[i].first;
                int to = freemap[j].first;
                freemap[j].second -= filesize;
                freemap[j].first += filesize;
                for (int k = 0; k < filesize; k++) {
                    assert(blockmap[from + k] >= 0);
                    assert(blockmap[to + k] < 0);
                    blockmap[to + k] = blockmap[from + k];
                    blockmap[from + k] = -1;
                }
                break;
            }
        }
    }
    return blockmap;
}

// Calculate the disk checksum
static ll checksum(const vector<int>& blockmap)
{
    ll csum = 0;
    for (size_t i = 0; i < blockmap.size(); i++) {
        if (blockmap[i] > 0) csum += i * blockmap[i];
    }
    return csum;
}

int main()
{
    string dmap;
    getline(cin, dmap);

    // Get maps of blocks, free ranges and file ranges
    vector<int> blockmap;
    vector<pair<int, int>> freemap;
    vector<pair<int, int>> filemap;
    bool freeblock = false;
    for (char c : dmap) {
        int start = blockmap.size();
        if (freeblock) {
            for (int i = 0; i < c - '0'; i++) {
                blockmap.push_back(-1);
            }
            if (c != '0') {
                freemap.emplace_back(start, c - '0');
            }
            freeblock = false;
        } else {
            assert(c != '0');
            for (int i = 0; i < c - '0'; i++) {
                blockmap.push_back(filemap.size());
            }
            filemap.emplace_back(start, c - '0');
            freeblock = true;
        }
    }

    cout << "Part 1: " << checksum(compact_1(blockmap)) << endl;
    cout << "Part 2: " << checksum(compact_2(blockmap, freemap, filemap)) << endl;

    return 0;
}
