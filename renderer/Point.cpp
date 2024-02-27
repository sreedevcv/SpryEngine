#include "Point.hpp"

spry::Point::Point()
{
}

spry::Point::Point(std::span<glm::vec3> points)
{
    load(points);
}

void spry::Point::load(std::span<glm::vec3> points)
{
    int format[] = {3};
    std::span<float> p {(float*)points.data(), points.size() * 3};
    m_mesh.load_data(p, format);
}

void spry::Point::draw()
{
    m_mesh.draw(GL_POINTS);
}

void spry::Point::set_point_size(float size)
{
    glPointSize(size);
}
