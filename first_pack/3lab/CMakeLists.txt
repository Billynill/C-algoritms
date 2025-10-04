cmake_minimum_required(VERSION 3.10)
project(3lab C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -g")

find_library(MATH_LIB m)

# Основная программа
add_executable(3lab
        src/main.c
        src/math_oper.c
        src/validate.c
)
target_include_directories(3lab PRIVATE src)
target_link_libraries(3lab ${MATH_LIB})

# Тесты
add_executable(3lab_tests
        tests/tests.c
        src/math_oper.c
        src/validate.c
)
target_include_directories(3lab_tests PRIVATE src)
target_link_libraries(3lab_tests ${MATH_LIB})

add_custom_target(run_tests
        COMMAND $<TARGET_FILE:3lab_tests>
        DEPENDS 3lab_tests
        COMMENT "Running tests..."
)