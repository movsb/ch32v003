enable_language(ASM)

file(GLOB SRC *.c *.cpp *.S)
add_library(stub ${SRC})
target_include_directories(stub PRIVATE ../include)
target_link_libraries(stub PRIVATE peripheral)
