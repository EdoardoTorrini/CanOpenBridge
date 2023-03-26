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
    "include"
    "include/utils"
)

add_library(can_open src/can_open.cpp)
add_library(node_ppm src/can_node_ppm.cpp)
add_library(node_cst src/can_node_cst.cpp)

add_executable(main main.cpp)

target_link_libraries(node_ppm can_open)
target_link_libraries(node_cst can_open)

target_link_libraries( main
    node_ppm
    node_cst
)

install(TARGETS 
    main
    DESTINATION ./$(PROJECT_NAME)
)
set(CMAKE_BUILD_TYPE Debug)