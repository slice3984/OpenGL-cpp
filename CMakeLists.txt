cmake_minimum_required(VERSION 3.26)
project("OpenGL")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/Linking/glew-2.1.0")
set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)

add_executable(OpenGL main.cpp)

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Linking/include)

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 GLEW::GLEW glfw)