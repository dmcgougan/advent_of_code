/*
 * Problem 5, Advent of Code 2019
 * Danjel McGougan
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

class intcode
{
public:
    enum result_t
    {
        INPUT,
        OUTPUT,
        HALT
    };

    intcode(const vector<int>& prg) : mem(prg) { }

    result_t run()
    {
        size_t msize = mem.size();
        for (;;) {
            // Get opcode
            assert(pc < msize);
            unsigned opcode = mem[pc++];
            unsigned modes = opcode / 100;
            opcode = opcode % 100;

            // Special handling for the halt opcode
            if (opcode == 99) return HALT;

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
                inp_addr = p[0];
                return INPUT;
            case 4:  // out
                out_value = p[0];
                return OUTPUT;
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

    void set_input(int input) { mem[inp_addr] = input; }

    int get_output() const { return out_value; }

private:
    vector<int> mem;
    size_t pc = 0;
    int inp_addr = 0;
    int out_value = 0;
};

static int get_thrust_p1(const vector<int>& prg, const array<int, 5>& phase)
{
    int signal = 0;
    for (size_t i = 0; i < phase.size(); ++i) {
        intcode ic(prg);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(phase[i]);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(signal);
        assert(ic.run() == intcode::OUTPUT);
        signal = ic.get_output();
        assert(ic.run() == intcode::HALT);
    }
    return signal;
}

static int get_thrust_p2(const vector<int>& prg, const array<int, 5>& phase)
{
    // Create the computers
    vector<intcode> ic;
    for (size_t i = 0; i < phase.size(); ++i) ic.emplace_back(prg);

    // Initialise
    array<intcode::result_t, 5> state = {};
    for (size_t i = 0; i < ic.size(); ++i) {
        assert(ic[i].run() == intcode::INPUT);
        ic[i].set_input(phase[i]);
        assert((state[i] = ic[i].run()) == intcode::INPUT);
    }
    ic[0].set_input(0);
    state[0] = ic[0].run();

    // Run until done
    for (;;) {
        bool ran = false;
        for (size_t i = 0; i < ic.size(); ++i) {
            int n = (i + 1) % 5;
            if (state[i] == intcode::OUTPUT && state[n] == intcode::INPUT) {
                ic[n].set_input(ic[i].get_output());
                state[i] = ic[i].run();
                state[n] = ic[n].run();
                ran = true;
            }
        }
        if (!ran) break;
    }

    // Check state
    for (size_t i = 0; i < 4; ++i) {
        assert(state[i] == intcode::HALT);
    }
    assert(state[4] == intcode::OUTPUT);

    // Get last output signal
    int signal = ic[4].get_output();
    assert(ic[4].run() == intcode::HALT);

    return signal;
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
    int part1 = INT_MIN;
    {
        array<int, 5> phase = {0, 1, 2, 3, 4};
        do {
            // Check thrust
            part1 = max(part1, get_thrust_p1(prg, phase));
        } while (next_permutation(phase.begin(), phase.end()));
    }

    // Solve part 2
    int part2 = INT_MIN;
    {
        array<int, 5> phase = {5, 6, 7, 8, 9};
        do {
            // Check thrust
            part2 = max(part2, get_thrust_p2(prg, phase));
        } while (next_permutation(phase.begin(), phase.end()));
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
