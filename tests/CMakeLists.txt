cmake_minimum_required(VERSION 3.21)

set(port _stuff/ports/ch32v00x)

# 包含公共命令
include(${port}/CMakeLists.txt)

# 创建项目
project(ch32v00x)

# 包含要编译的库目录
add_subdirectory(${port}/peripheral)
add_subdirectory(${port}/stub)
add_subdirectory(${port}/ports)
add_subdirectory(_stuff/base/time)

# 添加目标及头文件、库引用
set(target "ch32v00x.elf")
add_executable(${target} main.cpp)
target_include_directories(${target} PRIVATE ${port}/include)
target_include_directories(${target} PRIVATE _stuff/base/include)
# ports 提供了设备对基础库的接口底层实现，需要放到最后
target_link_libraries(${target} peripheral stub time ports)
ch32v00x_add_target(${target})
