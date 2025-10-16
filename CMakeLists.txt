cmake_minimum_required(VERSION 3.28)

project(low-level_setup
 VERSION 1.0
 LANGUAGES CXX
)

# Force C++ 20 instead of C++ 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(dependencies)
add_subdirectory(src)