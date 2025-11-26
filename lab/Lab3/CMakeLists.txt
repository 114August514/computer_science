cmake_minimum_required(VERSION 3.14)
project(Lab3)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Debug and Release configurations
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Binary Tree Project
add_subdirectory(BinaryTree)

# Graph Project
add_subdirectory(graph)

# Optional: Add a target to build all projects
add_custom_target(all_projects
    DEPENDS BinaryTreeDemo GraphDemo
    COMMENT "Building all Lab3 projects"
)