#include "Sphere.hpp"

#include <vector>
#include <span>
#include <cmath>
#include <glm/glm.hpp>

void spry::Sphere::load(const float radius, const int sectors, const int stacks)
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<int> indices;

    float PI = glm::pi<float>();
    float length_inverse = 1.0f / radius;

    const float sector_step = 2 * PI / sectors;
    const float stack_step = PI / stacks;

    for (int i = 0; i <= stacks; i++) {
        float stack_angle = (PI / 2) - (i * stack_step);
        float xy = radius * glm::cos(stack_angle);
        float z = radius * glm::sin(stack_angle);

        for (int j = 0; j <= sectors; j++) {
            float sector_angle = j * sector_step;

            float x = xy * glm::cos(sector_angle);
            float y = xy * glm::sin(sector_angle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            float normal_x = x * length_inverse;
            float normal_y = y * length_inverse;
            float normal_z = z * length_inverse;

            normals.push_back(normal_x);
            normals.push_back(normal_y);
            normals.push_back(normal_z);

            /*  Texture coordinates
                float s = static_cast<float>(j) / sectors;
                float t = static_cast<float>(i) / stacks;
            */
        }

        for (int i = 0; i < stacks; i++) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; j++, k1++, k2++) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != stacks - 1) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

    vertices.shrink_to_fit();
    normals.shrink_to_fit();
    indices.shrink_to_fit();

    int format[] = {3};

    m_mesh.load_data(std::span<float> {vertices}, std::span<int> {indices}, std::span<int> {format});
}

void spry::Sphere::draw()
{
    m_mesh.draw();
}

// https://www.songho.ca/opengl/gl_sphere.html