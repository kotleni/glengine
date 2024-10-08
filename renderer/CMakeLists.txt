cmake_minimum_required(VERSION 3.15)
project(renderer)

include(../cmake/detect_platform.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED)

# macOS should get gl and sdl2 from frameworks
if(OSX)
    list(APPEND EXTRA_LIBS
            "-framework OpenGL"
            "-framework SDL2"
            )
endif()

add_library(${PROJECT_NAME} SHARED
	shader.cpp
	directional_light.cpp
	skybox.cpp
	camera.cpp
    renderer.cpp
	model.cpp
	mesh.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
    
	${CMAKE_SOURCE_DIR}/logger
	../common
    ./
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	${ASSIMP_LIBRARIES}
	${SDL2_LIBRARIES}
	GLEW::GLEW
	logger
)

# Link libraries
if(NOT OSX) # macOS should use GL from frameworks
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()