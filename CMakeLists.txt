cmake_minimum_required(VERSION 3.20)

project(CompressionTool
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(compressor
    src/main.cpp
    src/cli.cpp
    src/file_handler.cpp
    src/huffman.cpp
    src/huffman_tree.cpp
)

target_include_directories(compressor PRIVATE src)

target_compile_options(compressor PRIVATE -Wall -Wextra -Wpedantic -Werror)

include(CTest)

if(BUILD_TESTING)
    add_executable(compressor_tests
        tests/tests.cpp
        src/cli.cpp
        src/file_handler.cpp
        src/huffman.cpp
        src/huffman_tree.cpp
    )

    target_include_directories(compressor_tests PRIVATE src)

    target_compile_options(compressor_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)

    add_test(NAME compressor_tests COMMAND compressor_tests)
endif()