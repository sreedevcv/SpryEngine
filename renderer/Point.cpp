#include "Point.hpp"

spry::Point::Point()
{
}

spry::Point::Point(std::vector<glm::vec3>& points)
{
    load(points);
}

void spry::Point::load(std::vector<glm::vec3>& points)
{
    std::vector<int> format = {3};
    m_mesh.load_data(points, format);
}

void spry::Point::draw()
{
    m_mesh.draw(GL_POINTS);
}

void spry::Point::set_point_size(float size)
{
    glPointSize(size);
}
