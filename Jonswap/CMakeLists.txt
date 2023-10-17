cmake_minimum_required(VERSION 3.0)

project(JONSWAP)

# Set C++ version to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/Jonswap.cpp
    src/main.cpp
)

# Include directories
include_directories(src)
add_subdirectory(matplotplusplus)

# Executable
add_executable(main ${SOURCES})
target_link_libraries(main PUBLIC matplot)
