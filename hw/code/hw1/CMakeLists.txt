cmake_minimum_required(VERSION 3.31)
project(hw1 C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(Inc)

# 设置源码文件路径
file(GLOB_RECURSE SOURCES "Src/*.c" "Src/*.cpp")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})