cmake_minimum_required(VERSION 3.26)
project(cat)
set(EXECUTABLE_NAME test)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    source/test.cpp)
set(HEADERS
    PUBLIC ./include
)

# Create test executable
add_executable(${EXECUTABLE_NAME} test_program/main.cpp)

# Create cat library
add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

# Add include directories
target_include_directories(${PROJECT_NAME} ${HEADERS})

# Link to test executable
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${PROJECT_NAME})

# Set warning level
if(MSVC)
  # compile at warning level 4 + treat warnings as errors
  target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Werror)
endif()