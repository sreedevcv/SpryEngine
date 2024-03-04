#include "CubeMap.hpp"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

spry::CubeMap::CubeMap()
{
    m_shader.set_shader_code(
        R"(
            #version 330 core
            layout (location = 0) in vec3 position;

            out vec3 tex_coords;

            uniform mat4 projection;
            uniform mat4 view;

            void main() {
                tex_coords = position;
                gl_Position = projection * view * vec4(position, 1.0);
            }
        )",
        R"(
            #version 330 core
            
            in vec3 tex_coords;
            out vec4 frag_color;

            uniform samplerCube skybox;

            void main() {
                frag_color = texture(skybox, tex_coords);
            }
        )");

    std::vector<float> skyboxVertices = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    std::vector<int> FORMAT = { 3 };
    m_mesh.load_data(skyboxVertices, FORMAT);
}

void spry::CubeMap::load_cube_map(const char* texture_paths[])
{
    stbi_set_flip_vertically_on_load(false);
    
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_id);

    int width, height, channel_count;
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(texture_paths[i], &width, &height, &channel_count, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "ERROR::CUBEMAP::Failed to load texture image: " << texture_paths[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void spry::CubeMap::draw(const glm::mat4& view, const glm::mat4& projection)
{
    glDepthMask(GL_FALSE);
    m_shader.use();
    m_shader.set_uniform_matrix("view", view);
    m_shader.set_uniform_matrix("projection", projection);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_id);
    m_mesh.draw();
    glDepthMask(GL_TRUE);
}
