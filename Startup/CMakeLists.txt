file(GLOB SOURCES *.S)

# 启用汇编语言支持
enable_language(ASM)

add_library(startup ${SOURCES})
