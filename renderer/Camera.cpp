#include "Camera.hpp"

#include "utils.hpp"

spry::Camera::Camera(const int scr_width, const int scr_height)
    : m_width(scr_width)
    , m_height(scr_height)
{
    update_camera_vectors();
}

glm::mat4 spry::Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 spry::Camera::get_projection_matrix()
{
    return glm::perspective(glm::radians(m_zoom), (float)(m_width / m_height), 0.1f, 100.0f);
}

void spry::Camera::update_camera_vectors()
{
    glm::vec3 updated_front;
    updated_front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    updated_front.y = glm::sin(glm::radians(m_pitch));
    updated_front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front = glm::normalize(updated_front);
    m_right = glm::normalize(glm::cross(m_front, m_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void spry::Camera::process_movement(movement m, float delta_time)
{
    float velocity = m_speed * delta_time;

    switch (m) {
    case movement::FORWARD:
        m_position += m_front * velocity;
        break;

    case movement::BACKWARD:
        m_position -= m_front * velocity;
        break;

    case movement::LEFT:
        m_position -= m_right * velocity;
        break;

    case movement::RIGHT:
        m_position += m_right * velocity;
        break;
    }
}

void spry::Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch)
{
    // std::cout << x_offset << " " << y_offset << "|\n";

    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;
    // std::cout << m_yaw << " " << m_pitch << "\n";

    m_yaw += x_offset;
    m_pitch += y_offset;

    // printVec(m_front);
    std::cout << m_yaw << " " << m_pitch << "\n";
    // std::cout << x_offset << " " << y_offset << "|\n";

    if (constrain_pitch) {
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        } else if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

    update_camera_vectors();
}

void spry::Camera::process_mouse_scroll(float y_offset)
{
    m_zoom -= y_offset;

    if (m_zoom < 1.0f) {
        m_zoom = 1.0f;
    } else if (m_zoom > 45.0f) {
        m_zoom = 45.0f;
    }
}

// void spry::Camera::create(const int scr_width, const int scr_height) {
//     width = scr_width;
//     height = scr_height;
// }
