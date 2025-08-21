cmake_minimum_required(VERSION 3.16)

project(Visualizer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.1
)

FetchContent_MakeAvailable(SFML)

add_executable(Visualizer
    project/main.cpp
    project/maze.cpp
    project/astar.cpp
)

target_include_directories(Visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Visualizer PRIVATE SFML::Graphics)