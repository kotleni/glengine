# Project Definition
cmake_minimum_required(VERSION 3.15)
project(engine VERSION 0.1.0)

include(cmake/detect_platform.cmake)
include(cmake/compiler_config.cmake)

add_subdirectory(logger/)
add_subdirectory(renderer/)
add_subdirectory(rocketui/)
add_subdirectory(engine/)
add_subdirectory(launcher/)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)