/*
 * Problem 5, Advent of Code 2019
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

static void intcode_run(const vector<int>& prg, const vector<int>& input, vector<int>& output)
{
    vector<int> mem(prg);
    size_t msize = mem.size();
    size_t inp_ix = 0;
    size_t pc = 0;
    for (;;) {
        // Get opcode
        assert(pc < msize);
        unsigned opcode = mem[pc++];
        unsigned modes = opcode / 100;
        opcode = opcode % 100;

        // Special handling for the halt opcode
        if (opcode == 99) break;

        // Get parameters
        static const unsigned opcode_info[] = {
            (3 << 2) | 3,  // 1 - add <src> <src> <dst>
            (3 << 2) | 3,  // 2 - mul <src> <src> <dst>
            (0 << 2) | 1,  // 3 - inp <dst>
            (1 << 2) | 1,  // 4 - out <src>
            (3 << 2) | 2,  // 5 - jne <src> <pc>
            (3 << 2) | 2,  // 6 - je  <src> <pc>
            (3 << 2) | 3,  // 7 - lt  <src> <src> <dst>
            (3 << 2) | 3,  // 8 - eq  <src> <src> <dst>
        };
        assert(opcode >= 1 && opcode <= size(opcode_info));
        unsigned info = opcode_info[opcode - 1];
        unsigned psize = info & 3;  // number of parameters
        unsigned rw = info >> 2;    // read/write info for each parameter
        array<int, 3> p;
        for (size_t i = 0; i < psize; ++i, rw >>= 1) {
            unsigned mode = modes % 10;
            modes /= 10;
            assert(pc < msize);
            p[i] = mem[pc++];
            if (mode == 0) {
                assert(p[i] >= 0 && p[i] < int(msize));
                if (rw & 1) {
                    p[i] = mem[p[i]];
                }
            } else {
                assert(mode == 1);
                assert(rw & 1);
            }
        }

        switch (opcode) {
        case 1:  // add
            mem[p[2]] = p[0] + p[1];
            break;
        case 2:  // mul
            mem[p[2]] = p[0] * p[1];
            break;
        case 3:  // inp
            assert(inp_ix < input.size());
            mem[p[0]] = input[inp_ix++];
            break;
        case 4:  // out
            output.push_back(p[0]);
            break;
        case 5:  // jne
            if (p[0]) pc = p[1];
            break;
        case 6:  // je
            if (!p[0]) pc = p[1];
            break;
        case 7:  // lt
            mem[p[2]] = (p[0] < p[1]);
            break;
        case 8:  // eq
            mem[p[2]] = (p[0] == p[1]);
            break;
        default:
            assert(false);
        }
    }
}

int main()
{
    // Parse input
    vector<int> prg;
    for (int n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }

    // Solve part 1
    int part1;
    {
        vector<int> input = {1};
        vector<int> output;
        intcode_run(prg, input, output);
        assert(output.size() > 0);
        part1 = output.back();
    }

    // Solve part 2
    int part2;
    {
        vector<int> input = {5};
        vector<int> output;
        intcode_run(prg, input, output);
        assert(output.size() > 0);
        part2 = output.back();
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
