/*
 * Problem 17, Advent of Code 2019
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

int main()
{
    // Parse input
    vector<ll> prg;
    for (ll n; cin >> n;) {
        prg.push_back(n);
        cin.ignore(1);
    }

    // Common data
    static const array<int, 2> dirs[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    static const char robot[] = {'>', 'v', '<', '^'};
    vector<string> grid;
    int rows;
    int cols;

    // Solve part 1
    int part1 = 0;
    {
        intcode ic(prg);
        string line;
        while (ic.run() == intcode::OUTPUT) {
            int output = ic.get_output();
            if (output == '\n') {
                if (!line.empty()) {
                    assert(grid.empty() || line.size() == grid[0].size());
                    grid.emplace_back(move(line));
                    line.clear();
                }
            } else {
                line += char(output);
            }
        }
        assert(line.empty());
        rows = grid.size();
        cols = grid[0].size();
        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                if (grid[r][c] != '#') continue;
                bool found = true;
                for (auto [dr, dc] : dirs) {
                    if (grid[r + dr][c + dc] != '#') {
                        found = false;
                        break;
                    }
                }
                if (found) part1 += r * c;
            }
        }
    }

    // Solve part 2
    prg[0] = 2;
    int64_t part2 = 0;
    {
        // Find the robot
        int row = -1;
        int col = -1;
        int dir = -1;
        for (int r = 0; r < rows && row < 0; ++r) {
            for (int c = 0; c < cols; ++c) {
                int i;
                for (i = 0; i < 4; i++)
                    if (grid[r][c] == robot[i]) break;
                if (i < 4) {
                    row = r;
                    col = c;
                    dir = i;
                    break;
                }
            }
        }
        assert(row >= 0);

        // Build a list of instructions to traverse all scaffolding
        vector<string> instr;
        for (;;) {
            // Check left and right of current direction
            bool found = false;
            for (int d = (dir + 3) % 4, i = 0; i < 2; d = (d + 2) % 4, ++i) {
                auto [dr, dc] = dirs[d];
                int nr = row + dr;
                int nc = col + dc;
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] == '#') {
                    // Choose this direction; check how far we can go
                    int steps = 0;
                    for (;;) {
                        row = nr;
                        col = nc;
                        nr = row + dr;
                        nc = col + dc;
                        ++steps;
                        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) break;
                        if (grid[nr][nc] != '#') break;
                    }
                    string turn(1, (d == (dir + 3) % 4 ? 'L' : 'R'));
                    instr.push_back(turn + "," + to_string(steps));
                    dir = d;
                    found = true;
                    break;
                }
            }
            if (!found) break;
        }
        assert(instr.size() > 0);
        int instr_size = instr.size();

        // Generate possible ways to pick routines A, B and C
        using interval_t = array<int, 2>;
        array<interval_t, 3> cur_abc;
        vector<array<interval_t, 3>> abc_comb;
        auto gen_abc = [&](auto& self, int start = 0, int interval = 0) -> void {
            if (interval == 3) {
                // Remove impossible combinations
                int sum = 0;
                int max_size = 0;
                for (int i = 0; i < 3; ++i) {
                    int size = cur_abc[i][1] - cur_abc[i][0];
                    if (size == 5) {
                        int rsize = 0;
                        for (int j = cur_abc[i][0]; j < cur_abc[i][1]; ++j) {
                            rsize += instr[j].size();
                        }
                        if (rsize + 4 > 20) return;
                    }
                    sum += size;
                    max_size = max(max_size, size);
                }
                if ((instr_size - sum) / max_size > 7) return;
                abc_comb.push_back(cur_abc);
                return;
            }
            for (int s = start; s < instr_size - 1; ++s) {
                if (s == start + 1) continue;
                for (int size = 2; size <= 5; ++size) {
                    int end = s + size;
                    if (end == instr_size - 1) continue;
                    if (end > instr_size) break;
                    cur_abc[interval] = {s, end};
                    self(self, end, interval + 1);
                }
            }
        };

        // Check if a specific way of picking A, B and C works
        auto check_abc = [&](const array<interval_t, 3>& abc, string& result) -> bool {
            result.clear();
            // Check the gap before the specified interval (or before end)
            for (int interval = 0; interval < 4; ++interval) {
                // Assign routines to the gap
                int start = (interval == 0 ? 0 : abc[interval - 1][1]);
                int end = interval < 3 ? abc[interval][0] : instr_size;
                while (start < end) {
                    array<bool, 3> abc_mismatch = {};
                    array<bool, 3> abc_match = {};
                    bool found_match = false;
                    int done = 0;
                    for (int pos = 0; done < 3 && start + pos < end; ++pos) {
                        for (int i = 0; i < 3; ++i) {
                            if (abc_mismatch[i] || abc_match[i]) continue;
                            if (pos < abc[i][1] - abc[i][0] &&
                                instr[start + pos] == instr[abc[i][0] + pos]) {
                                if (pos + 1 == abc[i][1] - abc[i][0]) {
                                    abc_match[i] = true;
                                    found_match = true;
                                    ++done;
                                }
                            } else {
                                abc_mismatch[i] = true;
                                ++done;
                            }
                        }
                    }
                    if (!found_match) return false;
                    // Pick the first interval that matched
                    for (int i = 0; i < 3; ++i) {
                        if (!abc_match[i]) continue;
                        start += abc[i][1] - abc[i][0];
                        result += char(i + 'A');
                        if (interval < 3 || start < end) result += ',';
                        break;
                    }
                }
                if (interval == 3) continue;
                result += char(interval + 'A');
                if (interval != 2 || abc[2][1] != instr_size) result += ',';
            }
            if (result.size() > 20) return false;
            return true;
        };

        // Build the robot instructions
        gen_abc(gen_abc);
        string send;
        for (const auto& abc : abc_comb) {
            string result;
            if (check_abc(abc, result)) {
                send += result + '\n';
                for (int i = 0; i < 3; ++i) {
                    for (int pos = abc[i][0]; pos < abc[i][1]; ++pos) {
                        send += instr[pos];
                        if (pos + 1 < abc[i][1]) send += ',';
                    }
                    send += '\n';
                }
                send += "n\n";
                break;
            }
        }
        assert(!send.empty());

        // Send the instructions
        intcode ic(prg);
        for (char ch : send) {
            intcode::result_t r;
            while ((r = ic.run()) == intcode::OUTPUT)
                ;
            assert(r == intcode::INPUT);
            ic.set_input(ch);
        }

        // Get the result (last output value before halt)
        for (intcode::result_t r; (r = ic.run()) != intcode::HALT;) {
            assert(r == intcode::OUTPUT);
            part2 = ic.get_output();
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
