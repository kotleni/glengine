cmake_minimum_required(VERSION 3.15)
project(renderer)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED)

# macOS should get gl and sdl2 from frameworks
if(MACOSX)
    list(APPEND EXTRA_LIBS
            "-framework OpenGL"
            "-framework SDL2"
            )
endif()

add_library(${PROJECT_NAME}
    renderer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
    
    include/
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)


# Link libraries
if(NOT MACOSX) # macOS should use GL from frameworks
	target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()

# Standard Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
	${SDL2_LIBRARIES}
	dl
)