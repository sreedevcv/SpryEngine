#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Camera {
private:
    const int width;
    const int height;


    void update_camera_vectors();

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;

    // Options
    float speed = 10.0f;
    float mouse_sensitivity = 0.1f;
    float zoom = 45.0f;

    Camera(const int width, const int height);
    ~Camera() = default;

    glm::mat4 get_view_matrix();

    enum class movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    struct mouse_data_struct {
        float first_mouse = true;
        float last_x;
        float last_y;
        bool captured = false;
    } mouse_data;

    void process_movement(movement m, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true);
    void process_mouse_scroll(float y_offset);
};