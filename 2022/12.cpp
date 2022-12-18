/*
 * Problem 12, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using ll = int64_t;

// This is kind of Dijkstra's algorithm in reverse.
// We start at the end location and calculate the cost of reaching that end
// location from all places on the map.
static vector<vector<ll>>
dijkstra(const vector<string>& terrain, ll destX, ll destY)
{
    vector<vector<ll>> cost;
    vector<vector<bool>> costDone;
    ll height = terrain.size();
    ll width = terrain[0].size();

    // Initialize the data structures
    for (ll y = 0; y < height; ++y) {
        cost.emplace_back(width, INT64_MAX);
        costDone.emplace_back(width, false);
    }

    // Set cost of destination location to zero
    cost[destY][destX] = 0;

    // Calculate all costs using Dijkstra's algorithm
    ll locationsLeft = width * height;
    while (locationsLeft > 0) {
        // Find the location that has not been calculated and has minimum cost
        ll minY = 0;
        ll minX = 0;
        ll minDist = INT64_MAX;
        for (ll y = 0; y < height; ++y) {
            for (ll x = 0; x < width; ++x) {
                if (!costDone[y][x] && cost[y][x] < minDist) {
                    minDist = cost[y][x];
                    minY = y;
                    minX = x;
                }
            }
        }

        // The cost calculation for this location is now done
        ll y = minY;
        ll x = minX;
        costDone[y][x] = true;
        locationsLeft--;

        if (minDist == INT64_MAX) {
            // We have found all locations from where E can be reached
            break;
        }

        // Check if the surrounding location costs can decrease if we go through the
        // current location instead of going some other route
        char elevation = terrain[y][x];
        if (y > 0 && !costDone[y - 1][x] && terrain[y - 1][x] + 1 >= elevation && cost[y - 1][x] > minDist + 1) {
            cost[y - 1][x] = minDist + 1;
        }
        if (y + 1 < height && !costDone[y + 1][x] && terrain[y + 1][x] + 1 >= elevation && cost[y + 1][x] > minDist + 1) {
            cost[y + 1][x] = minDist + 1;
        }
        if (x > 0 && !costDone[y][x - 1] && terrain[y][x - 1] + 1 >= elevation && cost[y][x - 1] > minDist + 1) {
            cost[y][x - 1] = minDist + 1;
        }
        if (x + 1 < width && !costDone[y][x + 1] && terrain[y][x + 1] + 1 >= elevation && cost[y][x + 1] > minDist + 1) {
            cost[y][x + 1] = minDist + 1;
        }
    }

    return cost;
}

// Print a pretty guide for anyone who wants to find the shortest route to E
static void
printGuide(const vector<vector<ll>>& cost)
{
    ll height = cost.size();
    ll width = cost[0].size();

    for (ll y = 0; y < height; ++y) {
        for (ll x = 0; x < width; ++x) {
            char output = 'U';
            ll c = cost[y][x];
            if (c == INT64_MAX) {
                // We can not reach E if we start from here
                output = ' ';
            } else if (c == 0) {
                // This is E
                output = 'E';
            } else {
                if (y > 0 && cost[y - 1][x] + 1 == c) {
                    output = '^';
                } else if (y + 1 < height && cost[y + 1][x] + 1 == c) {
                    output = 'v';
                } else if (x > 0 && cost[y][x - 1] + 1 == c) {
                    output = '<';
                } else if (x + 1 < width && cost[y][x + 1] + 1 == c) {
                    output = '>';
                }
            }
            cout << output;
        }
        cout << endl;
    }
}

int main()
{
    vector<string> terrain;
    ll startX = -1;
    ll startY = -1;
    ll destX = -1;
    ll destY = -1;
    ll width = 0;
    ll height = 0;

    // Read terrain from file
    string line;
    while (getline(cin, line)) {
        assert(!line.empty());
        if (!width) {
            width = line.length();
            assert(line.length() == (size_t)width);
        }
        assert(line.length() == (size_t)width);
        size_t pos;
        if ((pos = line.find('S')) != string::npos) {
            startX = pos;
            startY = terrain.size();
            line[pos] = 'a';
        }
        if ((pos = line.find('E')) != string::npos) {
            destX = pos;
            destY = terrain.size();
            line[pos] = 'z';
        }
        terrain.push_back(line);
        cout << line << endl;
    }
    height = terrain.size();
    assert(startX >= 0 && startY >= 0 && destX >= 0 && destY >= 0);

    // Calculate a complete cost map
    // Each entry in this map contains the cost to go from there to E
    vector<vector<ll>> cost = dijkstra(terrain, destX, destY);

    // Print guide
    cout << endl;
    printGuide(cost);
    cout << endl;

    // Find the location with elevation 'a' and minimum cost
    ll minY = 0;
    ll minX = 0;
    ll minDist = INT64_MAX;
    for (ll y = 0; y < height; ++y) {
        for (ll x = 0; x < width; ++x) {
            if (terrain[y][x] == 'a' && cost[y][x] < minDist) {
                minDist = cost[y][x];
                minY = y;
                minX = x;
            }
        }
    }

    cout << "Part 1: " << cost[startY][startX] << endl;
    cout << "Part 2: " << cost[minY][minX] << endl;

    return 0;
}
