cmake_minimum_required(VERSION 3.5)
project(can_open_bridge)

if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# add directory with all the headers files
include_directories(
    "obj/include"
)

add_library(can_open src/can_open.cpp)