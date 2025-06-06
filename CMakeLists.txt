cmake_minimum_required(VERSION 3.27)
project(CHIP_8)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SRC_FILES
    src/main.cpp
    src/CPU.cpp
    src/Display.cpp)

add_executable(CHIP_8 ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
