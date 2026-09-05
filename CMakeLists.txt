cmake_minimum_required(VERSION 3.23)

project(cpu-rasterizer LANGUAGES CXX)

# Put built executables/DLLs in a predictable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL source is inside libs/SDL
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)

# Your executable
add_executable(cpu-rasterizer
    src/main.cpp
    src/display/Display.cpp
    src/graphics/Rasterizer.cpp
)

# cpu-rasterizer uses SDL3
target_link_libraries(cpu-rasterizer
    PRIVATE
        SDL3::SDL3
)

target_include_directories(cpu-rasterizer PRIVATE src)