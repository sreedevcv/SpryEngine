#include "Quad.hpp"

#include <vector>

spry::Quad::Quad()
{
    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };
    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3,
    };
    std::vector<int> format = {3, 3, 2};
    
    m_mesh.load_data(vertices, indices, format);
}

void spry::Quad::draw()
{
    m_mesh.draw();
}
