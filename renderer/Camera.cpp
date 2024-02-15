#include "Camera.hpp"

Camera::Camera(const int scr_width, const int scr_height)
    : width(scr_width),
      height(scr_height) 
{
	mouse_data.last_x = width / 2;
	mouse_data.last_y = height / 2;
	update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::update_camera_vectors() {
	glm::vec3 updated_font;
	updated_font.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	updated_font.y = std::sin(glm::radians(pitch));
	updated_font.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front = glm::normalize(updated_font);

	right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));
}


void Camera::process_movement(movement m, float delta_time) {
	float velocity = speed * delta_time;

	switch (m) {
	case movement::FORWARD:
		position += front * velocity;
		break;
	
	case movement::BACKWARD:
		position -= front * velocity;
		break;

	case movement::LEFT:
		position -= right * velocity;
		break;
	
	case movement::RIGHT:
		position += right * velocity;
		break;
	}
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch) {
	if (!mouse_data.captured) return;

	x_offset *= mouse_sensitivity;
	y_offset *= mouse_sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	if (constrain_pitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}

	// std::cout << x_offset << " " << y_offset << "\n";

	update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
	zoom -= y_offset;

	if (zoom < 0.1f) {
		zoom = 0.1f;
	} else if (zoom > 45.0f) {
		zoom = 45.0f;
	}
}

// void Camera::create(const int scr_width, const int scr_height) {
//     width = scr_width;
//     height = scr_height;
// }
