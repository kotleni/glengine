cmake_minimum_required(VERSION 3.15)
project(launcher)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(EXECUTABLE_NAME "game")

include(../cmake/detect_platform.cmake)

add_executable(${EXECUTABLE_NAME} 
    main.cpp
)