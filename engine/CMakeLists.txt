cmake_minimum_required(VERSION 3.15)
project(engine)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

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
add_library(${PROJECT_NAME}
    SHARED
	gui/consoleguielement.cpp

	engine.cpp
	basicgameobject.cpp
	resources_manager.cpp
	entry.cpp
)

# Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/thirdparty
	
	${CMAKE_SOURCE_DIR}/common
	${CMAKE_SOURCE_DIR}/renderer
	${CMAKE_SOURCE_DIR}/rocketui
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

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./bin/")

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE RmlUi::RmlUi)


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

