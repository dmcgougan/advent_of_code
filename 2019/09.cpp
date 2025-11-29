/*
 * Problem 9, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
using ll = int64_t;

class intcode
{
public:
    enum result_t
    {
        INPUT,
        OUTPUT,
        HALT
    };

    intcode(const vector<ll>& prg)
    {
        for (size_t i = 0; i < prg.size(); ++i) write_mem(i, prg[i]);
    }

    result_t run()
    {
        for (;;) {
            // Get opcode
            ll op = read_mem(pc++);
            assert(op >= 0);
            unsigned opcode = op;
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
                (1 << 2) | 1,  // 9 - rel <val>
            };
            assert(opcode >= 1 && opcode <= size(opcode_info));
            unsigned info = opcode_info[opcode - 1];
            unsigned psize = info & 3;  // number of parameters
            unsigned rw = info >> 2;    // read/write info for each parameter
            array<ll, 3> p;
            for (size_t i = 0; i < psize; ++i, rw >>= 1) {
                unsigned mode = modes % 10;
                modes /= 10;
                p[i] = read_mem(pc++);
                switch (mode) {
                case 0:
                    if (rw & 1) {
                        p[i] = read_mem(p[i]);
                    }
                    break;
                case 1:
                    assert(rw & 1);
                    break;
                case 2:
                    if (rw & 1) {
                        p[i] = read_mem(p[i] + rel_base);
                    } else {
                        p[i] += rel_base;
                    }
                    break;
                default:
                    assert(false);
                }
            }

            switch (opcode) {
            case 1:  // add
                write_mem(p[2], p[0] + p[1]);
                break;
            case 2:  // mul
                write_mem(p[2], p[0] * p[1]);
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
                write_mem(p[2], p[0] < p[1]);
                break;
            case 8:  // eq
                write_mem(p[2], p[0] == p[1]);
                break;
            case 9:  // rel
                rel_base += p[0];
                break;
            default:
                assert(false);
            }
        }
    }

    void set_input(ll input) { write_mem(inp_addr, input); }

    ll get_output() const { return out_value; }

private:
    ll read_mem(ll addr) const
    {
        assert(addr >= 0);
        auto iter = mem.find(addr);
        if (iter == mem.end()) return 0;
        return iter->second;
    }

    void write_mem(ll addr, ll value)
    {
        assert(addr >= 0);
        mem[addr] = value;
    }

    unordered_map<ll, ll> mem;
    ll pc = 0;
    ll rel_base = 0;
    ll inp_addr = 0;
    ll out_value = 0;
};

int main()
{
    // Parse input
    vector<ll> prg;
    for (ll n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }

    // Solve part 1
    ll part1 = 0;
    {
        intcode ic(prg);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(1);
        assert(ic.run() == intcode::OUTPUT);
        part1 = ic.get_output();
        assert(ic.run() == intcode::HALT);
    }

    // Solve part 2
    ll part2 = 0;
    {
        intcode ic(prg);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(2);
        assert(ic.run() == intcode::OUTPUT);
        part2 = ic.get_output();
        assert(ic.run() == intcode::HALT);
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
