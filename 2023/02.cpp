/*
 * Problem 2, Advent of Code 2023
 * Danjel McGougan
 */

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
    int part1 = 0;
    int part2 = 0;

    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string s;
        ss >> s;
        int id;
        ss >> id;
        char colon;
        ss >> colon;
        bool possible = true;
        int max_red = 0, max_green = 0, max_blue = 0;
        while (!ss.eof()) {
            int red = 0, green = 0, blue = 0;
            while (!ss.eof()) {
                int count;
                ss >> count;
                string color;
                ss >> color;
                if (color.rfind("red", 0) == 0) red += count;
                if (color.rfind("green", 0) == 0) green += count;
                if (color.rfind("blue", 0) == 0) blue += count;
                if (color[color.size() - 1] == ';') break;
            }
            if (red > 12 || green > 13 || blue > 14) possible = false;
            if (red > max_red) max_red = red;
            if (blue > max_blue) max_blue = blue;
            if (green > max_green) max_green = green;
        }
        part1 += possible ? id : 0;
        part2 += max_red * max_green * max_blue;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
