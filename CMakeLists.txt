cmake_minimum_required(VERSION 3.27.0)
project(spryengine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(FetchContent)


# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG e2c92645460f680fd272fd2eed591efb2be7dc31
)
set(GLFW_USE_WAYLAND ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)


# IMGUI
# include_directories(deps/imgui)
# include_directories(deps/imgui/backends)
#
# add_library(imgui STATIC
#     deps/imgui/imgui.cpp
#     deps/imgui/imgui_demo.cpp
#     deps/imgui/imgui_draw.cpp
#     deps/imgui/imgui_tables.cpp
#     deps/imgui/imgui_widgets.cpp
#     deps/imgui/backends/imgui_impl_glfw.cpp
#     deps/imgui/backends/imgui_impl_opengl3.cpp
# )

# GLM
# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY https://github.com/g-truc/glm
#     GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b
# )
# set(GLM_BUILD_TESTS OFF)
# set(BUILD_SHARED_LIBS ON)
# FetchContent_MakeAvailable(glm)


find_package(OpenGL REQUIRED)

include_directories("include")
include_directories("math")
include_directories("common")
include_directories("renderer")

add_executable(
    spryengine
    "main.cpp"
    "renderer/glad.c"
    "math/Vec2.cpp"
    "math/Mat4.cpp"
    "common/Entity.cpp"
    "common/Rect.cpp"
    "math/Mat4.cpp"
    "renderer/Window.cpp"
    "renderer/Shader.cpp"
    "renderer/Cuboid.cpp"
    "renderer/Tetrahedron.cpp"
    "renderer/Texture.cpp"
)

target_link_libraries(
   spryengine
   OpenGL::GL
   glfw
)

# target_link_libraries(
#     spryengine
#     glfw
#     imgui
#     ${GLFW_LIBRARIES}
#     OpenGL::GL
#     glm::glm
# )
