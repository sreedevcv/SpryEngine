#include "Camera.hpp"

#include "utils.hpp"

spry::Camera::Camera()
{
    update_camera_vectors();
}

glm::mat4 spry::Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 spry::Camera::get_projection_matrix()
{
    return glm::perspective(glm::radians(m_zoom), (float)(m_width / m_height), 0.1f, 1000.0f);
}

void spry::Camera::set_screen_size(const int width, const int height)
{
    m_width = width;
    m_height = height;
}

void spry::Camera::update_camera_vectors()
{
    glm::vec3 updated_front;
    updated_front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    updated_front.y = std::sin(glm::radians(m_pitch));
    updated_front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
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
    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

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

    if (m_zoom < 0.1f) {
        m_zoom = 0.1f;
    } else if (m_zoom > 45.0f) {
        m_zoom = 45.0f;
    }
}