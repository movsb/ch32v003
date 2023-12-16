cmake_minimum_required(VERSION 3.21)

set(libroot ..)

# 包含公共命令
include(${libroot}/CMakeLists.txt)

# 创建项目
project(ch32v00x)

# 包含要编译的库目录
add_subdirectory(${libroot}/peripheral peripheral)
add_subdirectory(${libroot}/stub stub)

# 添加目标及头文件、库引用
set(target "ch32v00x.elf")
add_executable(${target} main.cpp)
target_include_directories(${target} PRIVATE ${libroot}/include)
target_link_libraries(${target} stub peripheral)
ch32v00x_add_target(${target})
