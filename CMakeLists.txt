cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

project(Roguelike)

find_package(fmt CONFIG REQUIRED)
find_package(libtcod CONFIG REQUIRED)

add_executable(Roguelike "src/Roguelike.cpp"  "src/Actor.h" "src/Actor.cpp" "src/Map.h" "src/Map.cpp" "src/Engine.h" "src/Engine.cpp" "src/Attacker.h" "src/Ai.h" "src/Destructible.h" "src/Destructible.cpp" "src/Attacker.cpp" "src/Ai.cpp" "src/Gui.h" "src/Gui.cpp" "src/Container.h" "src/Item.h" "src/Container.cpp" "src/Item.cpp" "src/Experience.h" "src/Experience.cpp")
target_link_libraries(Roguelike PRIVATE libtcod::libtcod fmt::fmt)

target_precompile_headers(Roguelike
  PUBLIC
    "src/precompiled_header.h"
)