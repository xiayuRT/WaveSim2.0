cmake_minimum_required(VERSION 3.27)

# Specify the C and C++ compilers
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")

# Specify the MinGW generator
set(CMAKE_GENERATOR "MinGW Makefiles")

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
