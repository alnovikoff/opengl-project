#include "render_util/camera.h"

Camera::Camera(glm::vec3 position) 
			: camera_pos(position),
			world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
			yaw(-90.0f),
			pitch(0.0f),
			speed(8.5f),
			zoom(45.0f)
{
	update_camera_vectors();
}

void Camera::update_camera_direction(double dx, double dy)
{
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if(pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update_camera_vectors();
}

void Camera::update_camera_position(CameraDirection dir, double dt)
{
	float velocity = (float)dt * speed;

	switch(dir)
	{
		case CameraDirection::FORWARD:
			camera_pos += camera_front * velocity;
			break;
		case CameraDirection::BACKWARD:
			camera_pos -= camera_front * velocity;
			break;
		case CameraDirection::RIGHT:
			camera_pos += camera_right * velocity;
			break;
		case CameraDirection::LEFT:
			camera_pos -= camera_right * velocity;
			break;
		case CameraDirection::UP:
			camera_pos += world_up * velocity;
			break;
		case CameraDirection::DOWN:
			camera_pos -= world_up * velocity;
			break;
	}
}

void Camera::update_camera_zoom(double dy)
{
	if(zoom >= 1.0f && zoom <= 45.0f)
	{
		zoom -= dy;
	}
	else if(zoom < 1.0f)
	{
		zoom = 1.0f;
	}
	else
	{
		zoom = 45.0f;
	}
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

float Camera::get_zoom()
{
	return zoom;
}

glm::vec3 Camera::get_camera_pos()
{
	return camera_pos;
}

void Camera::set_direction(glm::vec3 direction)
{
    direction = glm::normalize(direction);

    // Calculate the new yaw and pitch angles
    this->yaw = atan2(direction.y, direction.x);
    this->pitch = asin(direction.z);

    // Update the front vector
    this->update_camera_vectors();
}

void Camera::update_camera_vectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	camera_front = glm::normalize(front);
	camera_right = glm::normalize(glm::cross(camera_front, world_up));
	camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}

float Camera::distance_to_camera(const glm::vec3& position, const glm::vec3& _camera_position){
	return glm::length(_camera_position - position);
}