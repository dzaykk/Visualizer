#pragma once
#include "maze.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <array>
#include <functional> 
#include <cmath>      

class AStarStepper {
public:
    using Node = std::pair<int, Point>;

    explicit AStarStepper(const Maze& maze);

    bool step();

    std::optional<std::vector<Point>> getPath() const;

    const std::unordered_map<Point, int>& gScore()  const { return gScore_; }
    const std::unordered_map<Point, Point>& cameFrom()const { return cameFrom_; }
    const std::priority_queue<Node, std::vector<Node>, std::greater<>>& openSet() const { return openSet_; }
    const std::vector<Point>& closed() const { return closedOrder_; }

    void reset();

private:
    const Maze& maze_;
    Point start_{}, goal_{};

    std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet_;
    std::unordered_map<Point, Point> cameFrom_;
    std::unordered_map<Point, int> gScore_;
    std::unordered_set<Point> closed_;        
    std::vector<Point> closedOrder_;   

    bool finished_ = false;
    bool found_ = false;

    static int heuristic(Point a, Point b) {
        return std::abs(a.row - b.row) + std::abs(a.col - b.col);
    }
};
