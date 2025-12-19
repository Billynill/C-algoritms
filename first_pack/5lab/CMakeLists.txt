cmake_minimum_required(VERSION 3.10)
project(5lab C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Компиляционные флаги
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()

# Основная программа
add_executable(4lab
    src/main.c
    src/file_proc.c
    src/utils.c
)

target_include_directories(4lab PRIVATE src)




