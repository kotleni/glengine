# Project Definition
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_PARALLEL_LEVEL 3)
project(engine VERSION 0.1.0)

include(cmake/detect_platform.cmake)

add_subdirectory(renderer/)
add_subdirectory(rocketui/)

# Defines
# TODO: if/add is really needed?
if(ENABLE_TOOLS)
	add_compile_definitions(ENABLE_TOOLS)
endif()

# Prerequisites
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED)
find_package(RmlUi REQUIRED)

# macOS should get gl and sdl2 from frameworks
if(OSX)
    list(APPEND EXTRA_LIBS
            "-framework OpenGL"
            "-framework SDL2"
            )
endif()

# Source Files
add_executable(${PROJECT_NAME}
	src/gui/consoleguielement.cpp

	src/engine.cpp
	src/game_object.cpp
	src/resources_manager.cpp
	src/main.cpp
)

# Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/thirdparty
	
	common/
	renderer/
	rocketui/
)

# Compiler Options
target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall
	-Wextra
	-Wpedantic
	-Wunreachable-code
	-Wunused
	-Wignored-qualifiers
	-Wcast-align
	-Wformat-nonliteral
	-Wformat=2
	-Winvalid-pch
	-Wmissing-declarations
	-Wmissing-format-attribute
	-Wmissing-include-dirs
	-Wredundant-decls
	-Wswitch-default
	-Wodr
	-Wfatal-errors
	-fdiagnostics-color=always
	-pthread
	-pg
	-g)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)


# Link libraries
if(NOT OSX) # macOS should use GL from frameworks
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()

# Standard Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
	${SDL2_LIBRARIES}
	${OPENGL_INCLUDE_DIR} # FIXME: what? includes in linking?
	dl
	renderer
	rocketui
	)

# # Post Build Directories
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
# 	doc)

# # Post Build Copying
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	${CMAKE_SOURCE_DIR}/doc/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/doc)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)