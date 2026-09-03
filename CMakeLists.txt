cmake_minimum_required(VERSION 3.23)

project(renderer LANGUAGES CXX)

# Put built executables/DLLs in a predictable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL source is inside libs/SDL
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)

# Your executable
add_executable(renderer
    src/main.cpp
)

# renderer uses SDL3
target_link_libraries(renderer
    PRIVATE
        SDL3::SDL3
)