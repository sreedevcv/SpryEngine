#include "Camera.hpp"

#include "utils.hpp"

spry::Camera::Camera(const int scr_width, const int scr_height)
    : m_width(scr_width)
    , m_height(scr_height)
{
    mouse_data.last_x = m_width / 2;
    mouse_data.last_y = m_height / 2;
    update_camera_vectors();
}

spry::Mat4<float> spry::Camera::get_view_matrix()
{
    return spry::look_at(m_position, m_position + m_front, m_up);
}

spry::Mat4<float> spry::Camera::get_projection_matrix()
{
    return perspective_projection(0.1f, 100.0f, m_width / m_height, spry::radians(m_zoom));
}

void spry::Camera::update_camera_vectors()
{
    spry::Vec3<float> updated_font;
    updated_font.x = std::cos(spry::radians(m_yaw)) * std::cos(spry::radians(m_pitch));
    updated_font.y = std::sin(spry::radians(m_pitch));
    updated_font.z = std::sin(spry::radians(m_yaw)) * std::cos(spry::radians(m_pitch));
    m_front = spry::norm(updated_font);
    m_right = spry::norm(spry::cross(m_front, m_up));
    m_up = spry::norm(spry::cross(m_right, m_front));

    printVec(m_front);
    printVec(m_right);
    printVec(m_up);
	std::cout << "\n";
    

    // std::cout << "|\n";
    // printVec(m_front);
    // printVec(m_right);
    // printVec(m_up);
    // std::cout << "|\n";
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
    // if (!mouse_data.captured) return;

    x_offset *= m_mouse_sensitivity;
    y_offset *= m_mouse_sensitivity;
    // std::cout << m_yaw << " " << m_pitch << "\n";

    m_yaw += x_offset;
    m_pitch += y_offset;

    // std::cout << m_yaw << " " << m_pitch << "\n";
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
