cmake_minimum_required(VERSION 3.15)
project(rocketui)

include(../cmake/detect_platform.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(RmlUi REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
# macOS should get gl from frameworks
if(MACOSX)
    list(APPEND EXTRA_LIBS
            "-framework OpenGL"
            "-framework SDL2"
            )
endif()

add_library(${PROJECT_NAME}
    SHARED 
    RmlUi_Backend_SDL_GL3.cpp
    RmlUi_Platform_SDL.cpp
    RmlUi_Renderer_GL3.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
    
	../common
    ./
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${SDL2_LIBRARIES}
    GLEW::GLEW 
    RmlUi::RmlUi
)

# Link libraries
if(NOT OSX) # macOS should use GL from frameworks
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()