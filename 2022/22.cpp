/*
 * Problem 22, Advent of Code 2022
 * Danjel McGougan
 */

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = int64_t;

// The board and walking instructions
static vector<string> board;
static string instr;
static ll instr_pos;

// Row bounds for a specific column
static pair<ll, ll> row_bounds(ll col)
{
    ll row, begin, end;
    for (row = 0; row < ll(board.size()) && col >= ll(board[row].size()); ++row)
        ;
    for (; row < ll(board.size()) && col < ll(board[row].size()) && board[row][col] == ' '; ++row)
        ;
    begin = row;
    for (; row < ll(board.size()) && col < ll(board[row].size()) && board[row][col] != ' '; ++row)
        ;
    end = row;
    return make_pair(begin, end);
}

// Column bounds for a specific row
static pair<ll, ll> col_bounds(ll row)
{
    assert(row < ll(board.size()));
    const string& line = board[row];

    ll col, begin, end;
    for (col = 0; col < ll(line.size()) && line[col] == ' '; ++col)
        ;
    begin = col;
    for (; col < ll(line.size()) && line[col] != ' '; ++col)
        ;
    end = col;
    return make_pair(begin, end);
}

// Convert delta-row and delta-column to a direction
static ll dir(ll drow, ll dcol) { return dcol ? (dcol == 1 ? 0 : 2) : (drow == 1 ? 1 : 3); }

// Convert a direction to delta-row and delta-column
static pair<ll, ll> delta(ll dir)
{
    assert(dir >= 0 && dir < 4);
    constexpr pair<ll, ll> d[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    return d[dir];
}

// Get next position on the board for a given position and walking direction; part 1
static tuple<ll, ll, ll, ll> next_pos_p1(ll row, ll col, ll drow, ll dcol)
{
    if (drow != 0) {
        auto [begin, end] = row_bounds(col);
        row += drow;
        if (row < begin) row = end - 1;
        if (row >= end) row = begin;
    } else {
        assert(dcol != 0);
        auto [begin, end] = col_bounds(row);
        col += dcol;
        if (col < begin) col = end - 1;
        if (col >= end) col = begin;
    }

    return make_tuple(row, col, drow, dcol);
}

// Get next position on the board for a given position and walking direction; part 2
static tuple<ll, ll, ll, ll> next_pos_p2(ll row, ll col, ll drow, ll dcol)
{
    constexpr ll CUBE_SIDE = 50;

    // Cube face information; tells us where we appear when going
    // off the edge of a face and which direction we will have.
    // Yes, I used a piece of paper and scissors.
    constexpr struct
    {
        ll src_c_row;
        ll src_c_col;
        ll src_dir;
        ll dst_c_row;
        ll dst_c_col;
        ll dst_dir;
        bool swap_row_col;
    } face_info[] = {
        {0, 1, 2, 2, 0, 0, false},
        {0, 1, 3, 3, 0, 0, true},
        {0, 2, 0, 2, 1, 2, false},
        {0, 2, 1, 1, 1, 2, true},
        {0, 2, 3, 3, 0, 3, false},
        {1, 1, 0, 0, 2, 3, true},
        {1, 1, 2, 2, 0, 1, true},
        {2, 0, 2, 0, 1, 0, false},
        {2, 0, 3, 1, 1, 0, true},
        {2, 1, 0, 0, 2, 2, false},
        {2, 1, 1, 3, 0, 2, true},
        {3, 0, 0, 2, 1, 3, true},
        {3, 0, 1, 0, 2, 1, false},
        {3, 0, 2, 0, 1, 1, true},
    };

    if ((drow == 1 && row % CUBE_SIDE < CUBE_SIDE - 1) || (drow == -1 && row % CUBE_SIDE > 0) ||
        (dcol == 1 && col % CUBE_SIDE < CUBE_SIDE - 1) || (dcol == -1 && col % CUBE_SIDE > 0))
    {
        // Staying inside a cube face
        row += drow;
        col += dcol;
    } else {
        // Going off a face edge
        ll src_c_row = row / CUBE_SIDE;
        ll src_c_col = col / CUBE_SIDE;
        ll src_dir = dir(drow, dcol);
        ll dst_dir = -1;
        for (const auto& f : face_info) {
            if (src_c_row == f.src_c_row && src_c_col == f.src_c_col && src_dir == f.src_dir) {
                ll row_off = row % CUBE_SIDE;
                ll col_off = col % CUBE_SIDE;
                if (f.swap_row_col) {
                    // Changing 2D dimension
                    swap(row_off, col_off);
                } else {
                    // Invert the row offset
                    row_off = (CUBE_SIDE - 1 - row_off);
                }
                row = f.dst_c_row * CUBE_SIDE + row_off;
                col = f.dst_c_col * CUBE_SIDE + col_off;
                dst_dir = f.dst_dir;
                break;
            }
        }
        if (dst_dir >= 0) {
            tie(drow, dcol) = delta(dst_dir);
        } else {
            row += drow;
            col += dcol;
        }
    }

    return make_tuple(row, col, drow, dcol);
}

// Get next walking distance or -1 for EOF
static ll get_distance()
{
    if (instr_pos >= ll(instr.size())) return -1;
    char* end;
    ll walk = strtoll(instr.c_str() + instr_pos, &end, 10);
    assert(end - instr.c_str() > instr_pos);
    instr_pos = end - instr.c_str();
    return walk;
}

// Get next turn; L, R or E for EOF
static char get_turn()
{
    if (instr_pos >= ll(instr.size())) return 'E';
    return instr[instr_pos++];
}

// Walk the board
static ll run(ll part)
{
    // Starting position
    ll row = 0;
    ll col = col_bounds(row).first;
    ll drow = 0;
    ll dcol = 1;

    instr_pos = 0;

    for (;;) {
        // Walk
        ll dist = get_distance();
        if (dist < 0) break;
        for (ll m = 0; m < dist; m++) {
            auto [nrow, ncol, ndrow, ndcol] =
                (part == 1 ? next_pos_p1(row, col, drow, dcol) : next_pos_p2(row, col, drow, dcol));
            if (board[nrow][ncol] == '#') break;
            row = nrow;
            col = ncol;
            drow = ndrow;
            dcol = ndcol;
        }

        // Turn
        char d = get_turn();
        if (d == 'E') break;
        tie(drow, dcol) = delta((dir(drow, dcol) + (d == 'R' ? 1 : 3)) % 4);
    }

    return 1000 * (row + 1) + 4 * (col + 1) + dir(drow, dcol);
}

int main()
{
    // Parse the input
    string line;
    while (getline(cin, line)) {
        if (line.empty()) break;
        board.push_back(line);
    }
    assert(getline(cin, line));
    instr = line;
    assert(!getline(cin, line));

    // Run both parts
    cout << "Part 1: " << run(1) << endl;
    cout << "Part 2: " << run(2) << endl;

    return 0;
}
