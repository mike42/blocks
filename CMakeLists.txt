cmake_minimum_required(VERSION 3.15)
project(blocks)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set (APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
set (APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")

include_directories(src)

add_executable(blocks
        src/blocks_game.cpp
        src/blocks_game.h
        src/blocks_shape.cpp
        src/blocks_shape.h
        src/main.cpp)

target_link_libraries(blocks asan curses)
