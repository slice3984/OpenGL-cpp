cmake_minimum_required(VERSION 3.26)
project("OpenGL")

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGL main.cpp
        Linking/lib/glad.c)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Linking/include)

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glfw)