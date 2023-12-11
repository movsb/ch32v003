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
set(COMMON_FLAGS    "-march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -Wall -Wextra -Wlogical-op -g")
set(CMAKE_C_FLAGS   "${COMMON_FLAGS} -std=gnu17")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++17")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

# 使工具链测试编译通过
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

enable_language(C)
enable_language(CXX)
enable_language(ASM)

# 自定义一个包含常规生成方式的函数
# name 目前只应该是： xxx.elf 格式
function(ch32v00x_add_target name)
	string(REGEX MATCH "^(.+)\\.elf$" discard ${name})
	if ("${CMAKE_MATCH_1}" STREQUAL "")
		message(FATAL_ERROR "invalid target name ${name}. should be in the format of xxx.elf")
	endif()
	
	set(basename "${CMAKE_MATCH_1}")

	# 添加主生成目标及头文件、库引用
	target_link_options(${name} PRIVATE -T "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/stub/ch32v00x.ld" -nostartfiles -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs -Wl,-Map,${basename}.map)

	# 一些自定义的生成目标：二进制文件、列表文件、大小文件等
	add_custom_target(${basename}.hex ALL riscv-none-embed-objcopy -O ihex ${name} ${basename}.hex)
	add_custom_target(${basename}.lst ALL riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw ${name} > ${basename}.lst)
	add_custom_target(${basename}.siz ALL riscv-none-embed-size --format=berkeley ${name})
endfunction()
