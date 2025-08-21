#include "maze.hpp"
#include "astar.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    Maze maze;
    AStarStepper astar(maze);

    constexpr int CELL = 15;
    constexpr int STEP_DELAY_MS = 50; 

    sf::RenderWindow window(
        sf::VideoMode({ static_cast<unsigned int>(Maze::SIZE * CELL),
                        static_cast<unsigned int>(Maze::SIZE * CELL) }),
        "A* Pathfinding (M - regenerate) | GitHub: dzaykk"
    );
    window.setFramerateLimit(60);

    std::optional<std::vector<Point>> path;
    sf::Clock stepClock;

    while (window.isOpen()) {
        while (const std::optional ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* key = ev->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::M) {
                    maze.generate();
                    astar.reset();   
                    path.reset();
                    stepClock.restart();
                }
            }
        }

        if (!path && stepClock.getElapsedTime().asMilliseconds() >= STEP_DELAY_MS) {
            if (astar.step()) {
                path = astar.getPath();
            }
            stepClock.restart();
        }

        window.clear(sf::Color::Black);

        for (int r = 0; r < Maze::SIZE; ++r) {
            for (int c = 0; c < Maze::SIZE; ++c) {
                sf::RectangleShape rect(sf::Vector2f(CELL - 1.f, CELL - 1.f));
                rect.setPosition(sf::Vector2f(
                    static_cast<float>(c * CELL),
                    static_cast<float>(r * CELL)
                ));
                rect.setFillColor(
                    maze.at(r, c) == Maze::WALL ? sf::Color::Black : sf::Color(240, 240, 240)
                );
                window.draw(rect);
            }
        }

        for (const auto& p : astar.closed()) {
            sf::RectangleShape rect(sf::Vector2f(CELL - 1.f, CELL - 1.f));
            rect.setPosition(sf::Vector2f(
                static_cast<float>(p.col * CELL),
                static_cast<float>(p.row * CELL)
            ));
            rect.setFillColor(sf::Color(150, 150, 150));
            window.draw(rect);
        }

        auto openCopy = astar.openSet();
        while (!openCopy.empty()) {
            const auto [f, p] = openCopy.top();
            openCopy.pop();
            sf::RectangleShape rect(sf::Vector2f(CELL - 1.f, CELL - 1.f));
            rect.setPosition(sf::Vector2f(
                static_cast<float>(p.col * CELL),
                static_cast<float>(p.row * CELL)
            ));
            rect.setFillColor(sf::Color::Green);
            window.draw(rect);
        }

        if (path) {
            for (const auto& p : *path) {
                sf::RectangleShape rect(sf::Vector2f(CELL - 1.f, CELL - 1.f));
                rect.setPosition(sf::Vector2f(
                    static_cast<float>(p.col * CELL),
                    static_cast<float>(p.row * CELL)
                ));
                rect.setFillColor(sf::Color::Green);
                window.draw(rect);
            }
        }

        sf::RectangleShape rectStart(sf::Vector2f(CELL - 1.f, CELL - 1.f));
        rectStart.setPosition(sf::Vector2f(
            static_cast<float>(maze.start().col * CELL),
            static_cast<float>(maze.start().row * CELL)
        ));
        rectStart.setFillColor(sf::Color::Red);
        window.draw(rectStart);

        sf::RectangleShape rectGoal(sf::Vector2f(CELL - 1.f, CELL - 1.f));
        rectGoal.setPosition(sf::Vector2f(
            static_cast<float>(maze.goal().col * CELL),
            static_cast<float>(maze.goal().row * CELL)
        ));
        rectGoal.setFillColor(sf::Color::Cyan);
        window.draw(rectGoal);

        window.display();
    }
}