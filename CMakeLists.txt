# Define the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Define the project name, language, and version
project(ELS_Project
        VERSION 1.0
        LANGUAGES C # <-- HIER IST DIE KORREKTUR
)
# Set the C standard to use (e.g., C11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the 'include' directory globally so headers can be found
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add the source subdirectory to the build process
add_subdirectory(src)

# Optionally, add the test subdirectory
add_subdirectory(tests)