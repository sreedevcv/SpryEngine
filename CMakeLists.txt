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

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b
)
set(GLM_BUILD_TESTS OFF)
# set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(glm)

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
    "renderer/glad.c"
    "renderer/Camera.cpp"
    "renderer/PlaneMesh.cpp"

    "common/Entity.cpp"
    "common/Rect.cpp"
    "common/Logger.cpp"
)
target_link_libraries(renderer
    glfw
    glm::glm
)

include_directories("include")
add_executable(spryengine
    # "test/main.cpp"
    "test/test.cpp"
)

target_link_libraries(
   spryengine
   renderer
#    common
)