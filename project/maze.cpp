#include "maze.hpp"

Maze::Maze() : rng_(std::random_device{}()) {
    generate();
}

void Maze::generate() {
    for (auto& row : grid_)
        row.fill(WALL);
    carve(1, 1);
}

void Maze::carve(int r, int c) {
    grid_[r][c] = PATH;

    std::array<Point, 4> dirs = { {{ 2, 0 }, { -2, 0 }, { 0, 2 }, { 0, -2 }} };
    std::ranges::shuffle(dirs, rng_);

    for (auto [dr, dc] : dirs) {
        const int nr = r + dr, nc = c + dc;
        if (isInside(nr, nc) && grid_[nr][nc] == WALL) {
            grid_[r + dr / 2][c + dc / 2] = PATH;
            carve(nr, nc);
        }
    }
}