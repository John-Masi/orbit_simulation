cmake_minimum_required(VERSION 3.20)
project(orbit_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all source files
file(GLOB SOURCES "src/*.cpp")
add_executable(orbit_simulation ${SOURCES})

# Tell CMake where to find headers
target_include_directories(orbit_simulation PRIVATE ${CMAKE_SOURCE_DIR}/include)