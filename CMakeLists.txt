cmake_minimum_required(VERSION 3.21)

# 跨平台开发需要设置这些
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# 设置工具链
set(CMAKE_C_COMPILER    "riscv-none-embed-gcc")
set(CMAKE_CXX_COMPILER  "riscv-none-embed-g++")
set(CMAKE_CXX_COMPILER  "riscv-none-embed-g++")
set(CMAKE_AR            "riscv-none-embed-ar")
set(CMAKE_RANLIB        "riscv-none-embed-ranlib")

# 设置编译参数和选项
set(COMMON_FLAGS    "-march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -Wall -Wextra -Wlogical-op")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS} -std=gnu17")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++17")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

# 使工具链测试编译通过
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# 创建项目
project(ch32v00x)

# 包含要添加进项目的库目录
add_subdirectory(peripheral)
add_subdirectory(stub)

# 添加主生成目标及头文件、库引用
add_executable(ch32v00x.elf main.c test.cpp)
target_include_directories(ch32v00x.elf PRIVATE include)
target_link_libraries(ch32v00x.elf peripheral stub)
target_link_options(ch32v00x.elf PRIVATE -T ../stub/ch32v00x.ld -nostartfiles -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs -Wl,-Map,ch32v00x.map)

# 一些自定义的生成目标：二进制文件、列表文件、大小文件等
add_custom_target(ch32v00x.hex ALL riscv-none-embed-objcopy -O ihex ch32v00x.elf ch32v00x.hex)
add_custom_target(ch32v00x.lst ALL riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw ch32v00x.elf > ch32v00x.lst)
add_custom_target(ch32v00x.siz ALL riscv-none-embed-size --format=berkeley ch32v00x.elf)
