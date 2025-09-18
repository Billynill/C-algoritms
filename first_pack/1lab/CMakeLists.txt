cmake_minimum_required(VERSION 4.0)
project(1lab C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(main_program
        src/main.c
        src/logic.c
)
add_executable(tests
        tests/test.c
        src/logic.c
)