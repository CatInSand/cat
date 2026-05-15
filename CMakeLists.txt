cmake_minimum_required(VERSION 3.26)
project(cat)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES 
    source/main.cpp
    source/test.cpp)
set(HEADERS
    PUBLIC ./include
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME} ${HEADERS})