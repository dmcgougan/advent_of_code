/*
 * Problem 11, Advent of Code 2019
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
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
template <typename T, size_t size>
struct std::hash<array<T, size>>
{
    size_t operator()(const array<T, size>& a) const noexcept
    {
        size_t h = 0;
        for (const auto& v : a) {
            // Hash combine from boost
            h ^= hash<T>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

static void run(const vector<ll>& prg, unordered_map<array<int, 2>, int>& grid, int start_color)
{
    intcode ic(prg);
    int row = 0, col = 0, dir = 0;
    if (start_color) grid[{row, col}] = start_color;
    for (;;) {
        auto result = ic.run();
        if (result == intcode::HALT) break;
        assert(result == intcode::INPUT);
        int color = 0;
        auto iter = grid.find({row, col});
        if (iter != grid.end()) color = iter->second;
        ic.set_input(color);
        result = ic.run();
        if (result == intcode::HALT) break;
        assert(result == intcode::OUTPUT);
        int output = ic.get_output();
        grid[{row, col}] = output;
        assert(ic.run() == intcode::OUTPUT);
        int turn = ic.get_output();
        dir = (dir + (turn ? 1 : 3)) % 4;
        static const array<int, 2> dirs[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        row += dirs[dir][0];
        col += dirs[dir][1];
    }
}

int main()
{
    // Parse input
    vector<ll> prg;
    for (ll n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }

    // Solve part 1
    int part1;
    {
        unordered_map<array<int, 2>, int> grid;
        run(prg, grid, 0);
        part1 = grid.size();
    }
    cout << "Part 1: " << part1 << endl;

    // Solve part 2
    cout << "Part 2:" << endl;
    {
        unordered_map<array<int, 2>, int> grid;
        run(prg, grid, 1);
        int min_row = INT_MAX, max_row = INT_MIN;
        int min_col = INT_MAX, max_col = INT_MIN;
        for (auto [pos, _] : grid) {
            min_row = min(min_row, pos[0]);
            max_row = max(max_row, pos[0]);
            min_col = min(min_col, pos[1]);
            max_col = max(max_col, pos[1]);
        }
        for (int row = min_row; row <= max_row; ++row) {
            for (int col = min_col; col <= max_col; ++col) {
                auto iter = grid.find({row, col});
                cout << (iter != grid.end() && iter->second ? '#' : ' ');
            }
            cout << endl;
        }
    }

    return 0;
}
