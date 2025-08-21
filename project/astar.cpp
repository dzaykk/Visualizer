#include "astar.hpp"
#include <algorithm>

AStarStepper::AStarStepper(const Maze& maze)
    : maze_(maze)
{
    reset();
}

void AStarStepper::reset() {
    start_ = maze_.start();
    goal_ = maze_.goal();

    openSet_ = {};
    cameFrom_.clear();
    gScore_.clear();
    closed_.clear();
    closedOrder_.clear();
    finished_ = false;
    found_ = false;

    gScore_[start_] = 0;
    openSet_.push({ heuristic(start_, goal_), start_ });
}

bool AStarStepper::step() {
    if (finished_) return true;

    if (openSet_.empty()) {
        finished_ = true;
        found_ = false;
        return true;
    }

    const auto [f, current] = openSet_.top();
    openSet_.pop();

    if (closed_.contains(current)) {
        return false;
    }
    closed_.insert(current);
    closedOrder_.push_back(current);

    if (current == goal_) {
        finished_ = true;
        found_ = true;
        return true;
    }

    static constexpr std::array<Point, 4> dirs{ Point{1,0}, {-1,0}, {0,1}, {0,-1} };

    for (auto [dr, dc] : dirs) {
        Point nb{ current.row + dr, current.col + dc };
        if (!maze_.isInside(nb.row, nb.col)) continue;
        if (maze_.at(nb.row, nb.col) == Maze::WALL) continue;
        if (closed_.contains(nb)) continue;

        const int tentative = gScore_[current] + 1;
        auto it = gScore_.find(nb);
        if (it == gScore_.end() || tentative < it->second) {
            cameFrom_[nb] = current;
            gScore_[nb] = tentative;
            const int fScore = tentative + heuristic(nb, goal_);
            openSet_.push({ fScore, nb });
        }
    }

    return false;
}

std::optional<std::vector<Point>> AStarStepper::getPath() const {
    if (!finished_ || !found_) return std::nullopt;

    std::vector<Point> path;
    for (Point p = goal_; p != start_; p = cameFrom_.at(p))
        path.push_back(p);
    path.push_back(start_);
    std::ranges::reverse(path);
    return path;
}
