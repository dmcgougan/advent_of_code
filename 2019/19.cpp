/*
 * Problem 19, Advent of Code 2019
 * Danjel McGougan
 */

#include <array>
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

// Hash function for array
template <typename T, size_t N>
struct std::hash<array<T, N>>
{
    size_t operator()(const array<T, N>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

int main()
{
    // Parse input
    vector<ll> prg;
    for (ll n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }

    // Common code
    auto check = [&](int x, int y) -> bool {
        intcode ic(prg);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(x);
        assert(ic.run() == intcode::INPUT);
        ic.set_input(y);
        assert(ic.run() == intcode::OUTPUT);
        return ic.get_output();
    };

    // Solve part 1
    int part1 = 0;
    {
        for (int y = 0; y < 50; ++y) {
            for (int x = 0; x < 50; ++x) {
                part1 += check(x, y);
            }
        }
    }

    // Solve part 2
    int part2 = 0;
    {
        // Find the rough slope of the tractor beam
        int lo128, hi128;
        for (lo128 = 0; !check(lo128, 128); ++lo128)
            ;
        for (hi128 = lo128 + 1; check(hi128, 128); ++hi128)
            ;

        // Find the range of x values that are true for the specified y
        auto range = [&](int y) -> pair<int, int> {
            // Find the first x that is in the range
            int lo;
            for (lo = (lo128 - 1) * y / 128; !check(lo, y); ++lo)
                ;
            // Find the last x that is in the range
            int hi;
            for (hi = ((hi128 + 1) * y + 127) / 128; !check(hi, y); --hi)
                ;
            assert(!check(lo - 1, y) && check(lo, y));
            assert(check(hi, y) && !check(hi + 1, y));
            return {lo, hi};
        };

        // Check that a 100x100 square fits at the given coordinates
        auto sqcheck = [&](int x, int y) -> bool {
            return check(x, y) && check(x + 99, y) && check(x, y + 99);
        };

        // Check if we can make the square fit at the given y
        auto ycheck = [&](int y, int& x_out) -> bool {
            auto [x_lo, x_hi] = range(y);
            bool fits = false;
            for (int x = x_lo; x <= x_hi - 99; ++x) {
                if (sqcheck(x, y)) {
                    x_out = x;
                    fits = true;
                    break;
                }
            }
            return fits;
        };

        // Binary search to find where the 100x100 square fits
        int x = 0, y_lo = 0, y = 1 << 14;
        while (y - y_lo > 1) {
            int m = y_lo + (y - y_lo) / 2;
            ycheck(m, x) ? y = m : y_lo = m;
        }
        assert(ycheck(y, x));

        // Do a small search for smaller y since ycheck seems to not be monotone
        int y0 = y;
        for (int step = 1; step < 10 && y0 >= step; ++step) {
            if (ycheck(y0 - step, x)) y = y0 - step;
        }
        assert(ycheck(y, x));

        // Result
        part2 = 10000 * x + y;
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
