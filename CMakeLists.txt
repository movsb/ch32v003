cmake_minimum_required(VERSION 3.21)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER    "riscv-none-embed-gcc")
set(CMAKE_CXX_COMPILER  "riscv-none-embed-g++")
set(CMAKE_CXX_COMPILER  "riscv-none-embed-g++")
set(CMAKE_AR            "riscv-none-embed-ar")
set(CMAKE_RANLIB        "riscv-none-embed-ranlib")

set(COMMON_FLAGS    "-march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -std=gnu99 -Wall -Wextra -Wlogical-op")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

# 使工具链测试编译通过
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(ch32v00x)

add_subdirectory(Core)
add_subdirectory(Debug)
add_subdirectory(Peripheral)
add_subdirectory(Startup)
add_subdirectory(User)

add_executable(ch32v00x.elf main.c)
target_include_directories(ch32v00x.elf PRIVATE Debug)
target_include_directories(ch32v00x.elf PRIVATE Peripheral/inc)
target_include_directories(ch32v00x.elf PRIVATE Core)
target_include_directories(ch32v00x.elf PRIVATE User)
target_link_libraries(ch32v00x.elf startup core peripheral user debug_lib)
target_link_options(ch32v00x.elf PRIVATE "-T" "../Ld/link.ld" -nostartfiles -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs)
target_link_options(ch32v00x.elf PRIVATE -Wl,-Map,ch32v00x.map)

add_custom_target(ch32v00x.hex ALL riscv-none-embed-objcopy -O ihex ch32v00x.elf ch32v00x.hex)
add_custom_target(ch32v00x.lst ALL riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw ch32v00x.elf > ch32v00x.lst)
add_custom_target(ch32v00x.siz ALL riscv-none-embed-size --format=berkeley ch32v00x.elf)
