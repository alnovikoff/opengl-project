#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera(glm::vec3 position);

	void update_camera_direction(double dx, double dy);
	void update_camera_position(CameraDirection dir, double dt);
	void update_camera_zoom(double dy);

	glm::mat4 get_view_matrix();

	float get_zoom();
	glm::vec3 get_camera_pos();
	void set_direction(glm::vec3 direction);
	void update_camera_vectors();

private:

	glm::vec3 _camera_pos;

	glm::vec3 _camera_front;
	glm::vec3 _camera_up;
	glm::vec3 _camera_right;

	glm::vec3 _world_up;
	glm::vec3 _direction;

	float _yaw;
	float _pitch;
	float _speed;
	float _sensitivity;
	float _zoom;
};

#endif // CAMERA_H
