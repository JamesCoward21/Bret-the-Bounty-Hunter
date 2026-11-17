cmake_minimum_required(VERSION 3.20)

project(MyGameProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED IMPORTED_TARGET raylib)

# Create game executable
add_executable(MyGameProject src/main.cpp)

# Link raylib to the game
target_link_libraries(MyGameProject PRIVATE PkgConfig::RAYLIB)

# Include project header files
target_include_directories(MyGameProject PRIVATE include)
