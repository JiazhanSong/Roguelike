cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 20)

project(Roguelike)

find_package(fmt CONFIG REQUIRED)
find_package(libtcod CONFIG REQUIRED)

add_executable(Roguelike roguelike.cpp)
target_link_libraries(Roguelike PRIVATE libtcod::libtcod fmt::fmt)