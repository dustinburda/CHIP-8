cmake_minimum_required(VERSION 3.27)
project(CHIP_8)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(CHIP_8 main.cpp CPU.cpp Display.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
