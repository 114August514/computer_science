cmake_minimum_required(VERSION 3.15)
project(LC3_System C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra) # 开启警告

# ==========================================
# 1. 定义公共基础库（Header-Only Interface）
# 包含所有通用函数和数据结构的头文件（虚库，无需编译）
# ==========================================
add_library(lc3_common_lib INTERFACE)
# 指定头文件搜索路径
target_include_directories(lc3_common_lib INTERFACE ${CMAKE_SOURCE_DIR}/include)

# ==========================================
# 2. 定义功能模块库（Static Library）
# 实库，需要编译和链接
# ==========================================
# === 汇编器库 ===
add_library(lc3_assembler_lib STATIC
    src/assembler/assembler.cpp
        include/assembler/assembler.h
)

# 汇编器库依赖公共库
target_link_libraries(lc3_assembler_lib PUBLIC lc3_common_lib)
# 指定汇编器库的头文件搜索路径
target_include_directories(lc3_assembler_lib PUBLIC include)

# === 模拟器库 ===
add_library(lc3_simulator_lib STATIC
    src/simulator/lc3_vm.cpp
        include/simulator/lc3_vm.h
)

# 模拟器库依赖公共库
target_link_libraries(lc3_simulator_lib PUBLIC lc3_common_lib)
# 指定模拟器库的头文件搜索路径
target_include_directories(lc3_simulator_lib PUBLIC include)

# ==========================================
# 3. 生成可执行文件
# ==========================================
# === A. 汇编器可执行文件 ===
add_executable(lc3_asm src/main_asm.cpp)
target_link_libraries(lc3_asm PRIVATE lc3_assembler_lib)

# === B. 模拟器可执行文件 ===
add_executable(lc3_sim src/main_sim.cpp)
target_link_libraries(lc3_sim PRIVATE lc3_simulator_lib)

# ==========================================
# 4. 环境配置
# ==========================================
# 强制使用静态链接，防止脱离 CLion 运行时因缺少 DLL 而崩溃
if(MINGW OR CYGWIN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()