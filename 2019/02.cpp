/*
 * Problem 2, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static int run(const vector<int>& mem, int noun, int verb)
{
    vector<int> prg(mem);
    size_t size = prg.size();
    prg[1] = noun;
    prg[2] = verb;
    size_t pc = 0;
    for (;;) {
        assert(pc < size);
        int opcode = prg[pc];
        if (opcode == 99) break;
        assert(pc + 3 < size);
        size_t p0 = prg[pc + 1];
        size_t p1 = prg[pc + 2];
        size_t r = prg[pc + 3];
        assert(p0 < size && p1 < size && r < size);
        if (opcode == 1) {
            prg[r] = prg[p0] + prg[p1];
        } else {
            assert(opcode == 2);
            prg[r] = prg[p0] * prg[p1];
        }
        pc += 4;
    }
    return prg[0];
}

int main()
{
    // Parse input
    vector<int> prg;
    for (int n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }
    assert(prg.size() > 2);

    // Solve part 1
    int part1 = run(prg, 12, 2);

    // Solve part 2
    int part2 = 0;
    for (int n = 0; n < 100; ++n) {
        for (int v = 0; v < 100; ++v) {
            if (run(prg, n, v) == 19690720) {
                part2 = 100 * n + v;
                goto done;
            }
        }
    }
done:

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
