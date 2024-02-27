#pragma once

#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace spry {

class Camera {
private:
    int m_width;
    int m_height;
    float aspect_ratio;

    void update_camera_vectors();

public:
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 20.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    // Options
    float m_speed = 10.0f;
    float m_mouse_sensitivity = 0.1f;
    float m_zoom = 45.0f;

    Camera();
    ~Camera() = default;

    enum class movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    struct mouse_data_struct {
        float first_mouse = true;
        float last_x;
        float last_y;
        // bool captured = false;
    } mouse_data;

    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();
    
    void process_movement(movement m, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true);
    void process_mouse_scroll(float y_offset);
    
    void set_screen_size(const int width, const int height);
    void set_position(glm::vec3&& position);
};

}