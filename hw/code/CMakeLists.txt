cmake_minimum_required(VERSION 3.31)

# 从当前CMakeLists.txt的绝对路径中提取出最后的文件夹名
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${CURRENT_FOLDER_NAME} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# 添加头文件搜索路径（推荐在Inc中放置你的头文件），不过你在当前目录下直接放.h文件也行。
# 如果有自己的头文件目录，可以将其添加到这里。
include_directories(Inc .)

# 设置源码文件路径（推荐放置在Src目录下，不过现在允许你放置在除 .*cmake-build-.* 外任何路径下）
file(GLOB_RECURSE SOURCES "*.cpp" "*.c")
list(FILTER SOURCES EXCLUDE REGEX ".*cmake-build-.*")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})
