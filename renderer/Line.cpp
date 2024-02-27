#include "Line.hpp"

#include <span>

spry::Line::Line()
    : m_start(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_end(glm::vec3(1.0f, 1.0f, 1.0f))
{
    load_mesh();
}

spry::Line::Line(glm::vec3 start, glm::vec3 end)
    : m_start(start)
    , m_end(end)
{
    load_mesh();
}

void spry::Line::set_end_points(glm::vec3 start, glm::vec3 end)
{
    m_start = start;
    m_end = end;
    load_mesh();
}

void spry::Line::draw()
{
    m_mesh.draw(GL_LINES);
}

void spry::Line::load_mesh()
{
    float vertices[] = {
        m_start.x, m_start.y, m_start.z,
        m_end.x, m_end.y, m_end.z,
    };

    int format[] = {3};
    m_mesh.load_data(std::span<float>{vertices}, std::span<int>{format});
}

