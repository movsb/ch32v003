file(GLOB SRC *.c *.cpp)
add_library(peripheral ${SRC})
target_include_directories(peripheral PRIVATE ../include)
