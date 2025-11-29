/*
 * Problem 20, Advent of Code 2020
 * Danjel McGougan
 */

#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

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

// Transformation representing reflections and rotations
// The row coordinate grows downwards and col grows to the right
// Flip - flip the row axis
// Rotation 0 - 0 degrees
// Rotation 1 - 90 degrees clockwise
// Rotation 2 - 180 degrees clockwise
// Rotation 3 - 270 degrees clockwise
struct transform_t
{
    bool flip;
    int rot;

    // Constructors
    transform_t() : flip(false), rot(0) { }
    transform_t(bool f, int r) : flip(f), rot(r) { }

    // Apply the transform to coordinates row and col
    void apply(int& row, int& col, int row_size, int col_size = -1) const
    {
        if (col_size < 0) col_size = row_size;
        --row_size;
        --col_size;
        if (flip) row = row_size - row;
        if (rot & 1) {
            swap(row, col);
            if (rot & 2) {
                row = col_size - row;
            } else {
                col = row_size - col;
            }
        } else if (rot & 2) {
            row = row_size - row;
            col = col_size - col;
        }
    }

    // Create the inverse transform
    transform_t inverse() const { return transform_t(flip, (flip ? rot : -rot) & 3); }
};

// Create a transform that is equivalent to applying t0 and then t1
transform_t compose(const transform_t& t0, const transform_t& t1)
{
    return transform_t(t0.flip ^ t1.flip, (t1.rot + (t1.flip ? -t0.rot : t0.rot)) & 3);
}

static int tile_size;
static int grid_size;
static int image_size;
static unordered_map<array<int, 2>, pair<int, transform_t>> grid;
static unordered_map<int, vector<string>> tiles;

// Get an image pixel
static bool get_pixel(int image_row, int image_col)
{
    int grid_row = image_row / (tile_size - 2);
    int grid_col = image_col / (tile_size - 2);
    const auto& [tile_num, transform] = grid[{grid_row, grid_col}];
    const auto& tile = tiles[tile_num];
    int row = image_row % (tile_size - 2) + 1;
    int col = image_col % (tile_size - 2) + 1;
    transform.apply(row, col, tile_size);
    return tile[row][col] == '#';
}

int main()
{
    // Parse input
    string line;
    while (getline(cin, line)) {
        int tile_num;
        assert(sscanf(line.c_str(), "Tile %d:", &tile_num) == 1);
        auto& tile = tiles[tile_num];
        while (getline(cin, line) && !line.empty()) {
            assert(tile.empty() || line.size() == tile[0].size());
            tile.emplace_back(move(line));
        }
    }
    tile_size = tiles.begin()->second.size();

    // Determine number of tiles for one side of the square grid
    for (grid_size = 1; grid_size * grid_size < tiles.size(); ++grid_size)
        ;
    assert(grid_size * grid_size == tiles.size());
    image_size = grid_size * (tile_size - 2);

    // Get the north border of each tile after all 8 possible transformations
    unordered_map<int, array<int, 8>> tile_borders;                   // map tiles to borders
    unordered_map<int, vector<pair<int, transform_t>>> border_tiles;  // map borders to tiles
    for (const auto& [tile_num, tile] : tiles) {
        auto& border = tile_borders[tile_num];
        for (int i = 0, b = (1 << (tile_size - 1)); i < tile_size; ++i, b >>= 1) {
            // Do all 8 possible transformations
            for (int t = 0; t < 8; ++t) {
                int row = 0, col = i;
                transform_t(t & 4, t & 3).apply(row, col, tile_size);
                if (tile[row][col] == '#') border[t] |= b;
            }
        }
        for (int t = 0; t < 8; ++t) {
            auto& v = border_tiles[border[t]];
            v.push_back({tile_num, transform_t(t & 4, t & 3)});
            assert(v.size() <= 2);
        }
    }

    // Figure out how each tile connects to the surrounding tiles
    // Pick a random first tile and an arbitrary transform and build the grid around it
    unordered_map<int, tuple<int, int, transform_t, bool>> tile_pos;
    unordered_set<array<int, 2>> grid_pos_done;
    tile_pos[tiles.begin()->first] = {0, 0, transform_t(), false};
    grid_pos_done.insert({0, 0});
    for (;;) {
        // Find a tile that is placed but we have not placed its neighbors
        auto iter = tile_pos.begin();
        for (; iter != tile_pos.end() && get<3>(iter->second); ++iter)
            ;
        if (iter == tile_pos.end()) break;
        get<3>(iter->second) = true;

        // Find the neighbors of this tile by checking each orthogonal direction
        int tile_num = iter->first;
        auto& [row, col, tile_transform, done] = iter->second;
        static const tuple<int, int, transform_t, transform_t> dirs[] = {
            {-1, 0, transform_t(false, 0), transform_t(true, 0)},
            {1, 0, transform_t(true, 0), transform_t(false, 0)},
            {0, -1, transform_t(true, 1), transform_t(false, 3)},
            {0, 1, transform_t(false, 1), transform_t(true, 1)}};
        for (const auto& [dr, dc, dir_transform, neigh_transform] : dirs) {
            int nr = row + dr;
            int nc = col + dc;
            if (grid_pos_done.count({nr, nc})) continue;
            grid_pos_done.insert({nr, nc});

            // Get the border pattern for the current direction
            transform_t t(compose(dir_transform, tile_transform));
            int border = tile_borders[tile_num][t.flip << 2 | t.rot];

            // Check if we can find a tile with a matching border pattern
            auto iter = border_tiles.find(border);
            if (iter == border_tiles.end()) continue;
            const auto& v = iter->second;
            for (const auto& [tnum, transform] : v) {
                if (tnum == tile_num) continue;
                assert(tile_pos.find(tnum) == tile_pos.end());
                // Found the neighbour; store its location and transform
                tile_pos[tnum] = {nr, nc, compose(neigh_transform, transform), false};
            }
        }
    }
    assert(tile_pos.size() == grid_size * grid_size);

    // Place the tiles in a grid with zero based coordinates
    int min_row = INT_MAX, min_col = INT_MAX;
    for (const auto& [_, tup] : tile_pos) {
        min_row = min(min_row, get<0>(tup));
        min_col = min(min_col, get<1>(tup));
    }
    for (const auto& [tile_num, tup] : tile_pos) {
        const auto& [row, col, transform, _] = tup;
        grid[{row - min_row, col - min_col}] = {tile_num, transform};
    }

    // Solve part 1
    int64_t part1 = int64_t(grid[{0, 0}].first) * grid[{grid_size - 1, 0}].first *
        grid[{0, grid_size - 1}].first * grid[{grid_size - 1, grid_size - 1}].first;

    // Build the sea monster
    static const vector<string> sm_image = {"                  # ",
                                            "#    ##    ##    ###",
                                            " #  #  #  #  #  #   "};
    int sm_rows = sm_image.size();
    int sm_cols = sm_image[0].size();
    unordered_set<array<int, 2>> sm;
    for (int r = 0; r < sm_rows; ++r) {
        for (int c = 0; c < sm_cols; ++c) {
            if (sm_image[r][c] == '#') sm.insert({r, c});
        }
    }

    // Find sea monsters in the image and solve part 2
    // Reflect and rotate the monster instead of the image
    int image_count;
    int sm_count = 0;
    for (int t = 0; t < 8; ++t) {
        transform_t transform(t & 4, t & 3);
        image_count = 0;
        for (int irow = 0; irow < image_size; ++irow) {
            for (int icol = 0; icol < image_size; ++icol) {
                image_count += get_pixel(irow, icol);
                bool found = true;
                for (auto [mrow, mcol] : sm) {
                    transform.apply(mrow, mcol, sm_rows, sm_cols);
                    if (irow + mrow >= image_size || icol + mcol >= image_size ||
                        !get_pixel(irow + mrow, icol + mcol)) {
                        found = false;
                        break;
                    }
                }
                sm_count += found;
            }
        }
        if (sm_count) break;
    }
    int part2 = image_count - sm_count * sm.size();

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
