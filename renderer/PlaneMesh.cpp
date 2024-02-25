#include "PlaneMesh.hpp"

#include "Vec3.hpp"
#include <vector>
#include <span>

spry::PlaneMesh::PlaneMesh()
{
}

spry::PlaneMesh::PlaneMesh(float length, float breadth, int length_segments, int breadth_segments)
    : m_length(length)
    , m_breadth(breadth)
{
    load(length, breadth, length_segments, breadth_segments);
}

void spry::PlaneMesh::load(float length, float breadth, int length_segments, int breadth_segments)
{
    std::vector<float> vertices;
    float length_step = length / length_segments;
    float breadth_step = breadth / breadth_segments;

    for (int i = 0; i <= length_segments; i++) {
        for (int j = 0; j <= breadth_segments; j++) {
            Vec3<float> vertex;
            vertex.x = i * length_step;
            vertex.y = j * breadth_step;
            vertex.z = 0.0f;
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
        }
    }

    std::vector<int> indices;
    int count = 0;

    for (int i = 0; i < length_segments; i++) {
        for (int j = 0; j < breadth_segments; j++) {
            int v0 = i * (breadth_segments + 1) + j;
            int v1 = v0 + 1;
            int v2 = (i + 1) * (breadth_segments + 1) + j;
            int v3 = v2 + 1;

            // First tri
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            // Second tri
            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v3);
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    int format[] = { 3 };
    m_mesh.load_data(std::span<float> { vertices }, std::span<int> { indices }, std::span<int> { format });
}

void spry::PlaneMesh::draw()
{
    m_mesh.draw();
}
