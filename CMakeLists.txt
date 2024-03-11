cmake_minimum_required(VERSION 3.26)
project("OpenGL")

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_SOURCES
        external/imgui/imgui.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
)

add_executable(OpenGL src/main.cpp
        ${IMGUI_SOURCES}
        Linking/lib/glad.c
        src/Shader.cpp
        src/Shader.h
        src/Shader.h
        src/stb.cpp
        src/Camera.cpp
        src/Camera.h
        src/util.cpp
        src/util.h
        src/InputManager.h
        src/imgui_windows/ControlsWindow.h
        src/TexturedMeshData.h
        src/RenderEntity.h
        src/Renderer.cpp
        src/Renderer.h
        src/tiny_gltf.cc
        src/MeshTypes.h
        src/MaterialStore.cpp
        src/MaterialStore.h
        src/RenderQueue.cpp
        src/RenderQueue.h
        src/Model.h
        src/TextureStore.cpp
        src/TextureStore.h
        src/ModelStore.cpp
        src/ModelStore.h
        src/GltfModelImporter.cpp
        src/GltfModelImporter.h
        src/ModelGenerator.cpp
        src/ModelGenerator.h
)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/Linking/include
        ${CMAKE_SOURCE_DIR}/external/imgui
        ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glfw)