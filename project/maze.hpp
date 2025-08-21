#pragma once
#include <array>
#include <random>
#include <compare>
#include <cstddef>
#include <algorithm>
#include <utility>

struct Point {
    int row{}, col{};
    auto operator<=>(const Point&) const = default;
};

namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const noexcept {
            return (static_cast<size_t>(static_cast<uint32_t>(p.row)) << 32)
                ^ static_cast<uint32_t>(p.col);
        }
    };
}

class Maze {
public:
    static constexpr int SIZE = 41;
    static constexpr int WALL = 1;
    static constexpr int PATH = 0;

    Maze();

    void generate();
    int at(int r, int c) const { return grid_[r][c]; } 
    bool isInside(int r, int c) const {
        return r > 0 && c > 0 && r < SIZE - 1 && c < SIZE - 1;
    }

    Point start() const { return { 1, 1 }; }
    Point goal()  const { return { SIZE - 2, SIZE - 2 }; }

private:
    std::array<std::array<int, SIZE>, SIZE> grid_{};
    std::mt19937 rng_;

    void carve(int r, int c);
};
