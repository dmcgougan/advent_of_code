/*
 * Problem 21, Advent of Code 2019
 * Danjel McGougan
 */

#include <array>
#include <cassert>
#include <fstream>
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

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    assert(in);

    // Parse input
    vector<ll> prg;
    for (ll n; in >> n;) {
        prg.push_back(n);
        in.ignore(1);
    }

    // Common code
    auto runscript = [&](const string& script) -> ll {
        intcode ic(prg);
        size_t p = 0;
        while (p < script.size()) {
            auto r = ic.run();
            if (r == intcode::INPUT) {
                ic.set_input(script[p++]);
            } else {
                assert(r == intcode::OUTPUT);
            }
        }
        intcode::result_t r;
        ll ret = -1;
        while ((r = ic.run()) == intcode::OUTPUT) {
            ll v = ic.get_output();
            if (v >= 128) ret = v;
        }
        assert(r == intcode::HALT);
        return ret;
    };

    // Solve part 1
    ll part1 = runscript(
        "NOT C J\n"
        "AND A J\n"
        "AND D J\n"
        "NOT A T\n"
        "OR T J\n"
        "WALK\n");

    // Solve part 2
    ll part2 = runscript(
        "NOT A J\n"
        "AND D J\n"
        "NOT B T\n"
        "AND D T\n"
        "AND H T\n"
        "OR T J\n"
        "NOT C T\n"
        "AND D T\n"
        "AND H T\n"
        "OR T J\n"
        "RUN\n");

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
