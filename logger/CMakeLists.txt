cmake_minimum_required(VERSION 3.15)
project(logger)

include(../cmake/compiler_config.cmake)
include(../cmake/detect_platform.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_library(${PROJECT_NAME}
    SHARED 
    logger.cpp
)