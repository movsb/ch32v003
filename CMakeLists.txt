cmake_minimum_required(VERSION 3.21)

# 包含公共命令
include(CMakeListsCommon.txt)

# 创建项目
project(ch32v00x)

# 包含要编译的库目录
add_subdirectory(peripheral)
add_subdirectory(stub)

# 添加目标及头文件、库引用
set(target "ch32v00x.elf")
add_executable(${target} main.c test.cpp)
target_include_directories(${target} PRIVATE include)
target_link_libraries(${target} peripheral stub)
ch32v00x_add_target(${target})
