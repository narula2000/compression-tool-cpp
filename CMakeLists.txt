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
    src/huffman.cpp
)

target_compile_options(compressor PRIVATE -Wall -Wextra -Wpedantic -Werror)
