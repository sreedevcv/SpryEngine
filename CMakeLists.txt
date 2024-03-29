cmake_minimum_required(VERSION 3.27.0)
project(spryengine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# set_target_properties(spryengine
# PROPERTIES 
# RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})


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

#ASSIMP
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp
    GIT_TAG 01231d0e6001f555c81dcfcc6c581fa5797ccac9
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE) 
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE) 
set(ASSIMP_WARNINGS_AS_ERRORS ON CACHE BOOL "" FORCE) 
FetchContent_MakeAvailable(assimp)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b
)
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(glm)

# IMGUI
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG fbf45ad149b10ff8d9cb97aefe0dc5a9562fd66e
)
FetchContent_MakeAvailable(imgui)

include_directories(build/_deps/imgui-src)
include_directories(build/_deps/imgui-src/backends)

add_library(imgui STATIC
    build/_deps/imgui-src/imgui.cpp 
    build/_deps/imgui-src/imgui_demo.cpp 
    build/_deps/imgui-src/imgui_draw.cpp 
    build/_deps/imgui-src/imgui_tables.cpp 
    build/_deps/imgui-src/imgui_widgets.cpp
    build/_deps/imgui-src/backends/imgui_impl_glfw.cpp
    build/_deps/imgui-src/backends/imgui_impl_opengl3.cpp 
)

target_link_libraries(imgui glfw)

# MATH
# include_directories("math")
# add_library(math
#     "math/Vec2.cpp"
#     "math/Mat4.cpp"
#     "math/Transforms.cpp"
#     "math/Vec3.cpp"
# )

# COMMON
include_directories("common")
# add_library(common 
#     "common/Entity.cpp"
#     "common/Rect.cpp"
# )

# RENDERER
include_directories("renderer")
add_library(renderer 
    "renderer/Window.cpp"
    "renderer/Shader.cpp"
    "renderer/Cuboid.cpp"
    "renderer/Tetrahedron.cpp"
    "renderer/BasicMesh.cpp"
    "renderer/Texture.cpp"
    "renderer/glad.c"
    "renderer/Camera.cpp"
    "renderer/PlaneMesh.cpp"
    "renderer/Sphere.cpp"
    "renderer/Line.cpp"
    "renderer/Point.cpp"
    "renderer/Transform.cpp"
    "renderer/Mesh.cpp"
    "renderer/Model.cpp"
    # "renderer/Material.hpp"
    "renderer/Quad.cpp"
    "renderer/CubeMap.cpp"

    "common/Entity.cpp"
    "common/Rect.cpp"
    "common/Logger.cpp"
)
target_link_libraries(renderer
    glfw
    glm::glm
    assimp
)

include_directories("include")
add_executable(spryengine
    # "test/main.cpp"
    "examples/test.cpp"
    "examples/RubiksCube.cpp"
    "examples/objectOutlining.cpp"
    "examples/blending.cpp"
    "examples/textureRendering.cpp"
    "examples/cubemap.cpp"
    "examples/advanced_glsl.cpp"

    "examples/renderPoints.cpp"
    "examples/geometry_shader.cpp"
    "examples/shadows.cpp"
    "examples/tree.cpp"
)

target_link_libraries(
   spryengine
   renderer
   imgui
#    common
)