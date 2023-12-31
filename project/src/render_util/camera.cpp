#include "render_util/camera.h"

Camera::Camera(glm::vec3 position) 
			: _camera_pos(position),
			_world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
			_yaw(-90.0f),
			_pitch(0.0f),
			_speed(8.5f),
			_zoom(45.0f)
{
	update_camera_vectors();
}

void Camera::update_camera_direction(double dx, double dy)
{
	_yaw += dx;
	_pitch += dy;

	if (_pitch > 89.0f)
	{
		_pitch = 89.0f;
	}
	else if(_pitch < -89.0f)
	{
		_pitch = -89.0f;
	}

	update_camera_vectors();
}

void Camera::update_camera_position(CameraDirection dir, double dt)
{
	float velocity = (float)dt * _speed;

	switch(dir)
	{
		case CameraDirection::FORWARD:
			_camera_pos += _camera_front * velocity;
			break;
		case CameraDirection::BACKWARD:
			_camera_pos -= _camera_front * velocity;
			break;
		case CameraDirection::RIGHT:
			_camera_pos += _camera_right * velocity;
			break;
		case CameraDirection::LEFT:
			_camera_pos -= _camera_right * velocity;
			break;
		case CameraDirection::UP:
			_camera_pos += _world_up * velocity;
			break;
		case CameraDirection::DOWN:
			_camera_pos -= _world_up * velocity;
			break;
	}
}

void Camera::update_camera_zoom(double dy)
{
	if(_zoom >= 1.0f && _zoom <= 45.0f)
	{
		_zoom -= dy;
	}
	else if(_zoom < 1.0f)
	{
		_zoom = 1.0f;
	}
	else
	{
		_zoom = 45.0f;
	}
}

glm::mat4 Camera::get_view_matrix()
{
	return glm::lookAt(_camera_pos, _camera_pos + _camera_front, _camera_up);
}

float Camera::get_zoom()
{
	return _zoom;
}

glm::vec3 Camera::get_camera_pos()
{
	return _camera_pos;
}

void Camera::set_direction(glm::vec3 direction)
{
    direction = glm::normalize(direction);

    // Calculate the new yaw and pitch angles
    this->_yaw = atan2(direction.y, direction.x);
    this->_pitch = asin(direction.z);

    // Update the front vector
    this->update_camera_vectors();
}

void Camera::update_camera_vectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

	_camera_front = glm::normalize(front);
	_camera_right = glm::normalize(glm::cross(_camera_front, _world_up));
	_camera_up = glm::normalize(glm::cross(_camera_right, _camera_front));
}

float Camera::distance_to_camera(const glm::vec3& position, const glm::vec3& camera_position){
	return glm::length(camera_position - position);
}