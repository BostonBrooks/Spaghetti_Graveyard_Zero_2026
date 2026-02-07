cmake_minimum_required(VERSION 4.1)
project(Spaghetti_Graveyard_Zero_2026 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../games/game0)

add_executable(Spaghetti_Graveyard_Zero_2026 test/incremental_test.c
        engine/logic/bbIncludes.c
        engine/logic/bbBloatedPool.c
        engine/logic/bbDictionary.c)

target_include_directories(Spaghetti_Graveyard_Zero_2026  PRIVATE "./")

target_link_libraries(Spaghetti_Graveyard_Zero_2026 PRIVATE
        csfml-graphics
        csfml-window
        csfml-system
)