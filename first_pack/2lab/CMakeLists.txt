cmake_minimum_required(VERSION 3.16)
project(2lab LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(prime_utils STATIC
        src/prime_utils.c
)
target_include_directories(prime_utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(nthprime
        src/main.c
)
target_link_libraries(nthprime PRIVATE prime_utils)

if (UNIX AND NOT APPLE)
    find_library(M_LIB m)
    if (M_LIB)
        target_link_libraries(prime_utils PUBLIC m)
        target_link_libraries(nthprime PRIVATE m)
    endif()
endif()

add_executable(nthprime_tests
        tests/tests.c
)
target_link_libraries(nthprime_tests PRIVATE prime_utils)
if (UNIX AND NOT APPLE AND M_LIB)
    target_link_libraries(nthprime_tests PRIVATE m)
endif()

include(CTest)
if (BUILD_TESTING)
    add_test(NAME nthprime_smoke COMMAND nthprime_tests)
endif()


