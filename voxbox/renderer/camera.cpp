#include "camera.h"

using namespace glm;

Camera::Camera(GLint modelLoc, GLint viewLoc, GLint projLoc) {
	_modelLoc = modelLoc;
	_viewLoc = viewLoc;
	_projLoc = projLoc;

	_mouseSpeed = 0.05f;
	_speed = 25.0f;
	_position = vec3(64, 80, 64);
	_horizontalAngle = 3.14f;
	_verticalAngle = 0.0f;

	mat4 model = mat4(1.0f);
	glUniformMatrix4fv(_modelLoc, 1, GL_FALSE, value_ptr(model));

	mat4 view = lookAt(vec3(_position), vec3(0, 0, 0), vec3(0, 1, 0));;
	glUniformMatrix4fv(_viewLoc, 1, GL_FALSE, value_ptr(view));

	mat4 proj = perspective(radians(70.0f), 1280.0f / 720.0f, 1.0f, 400.0f);
	glUniformMatrix4fv(_projLoc, 1, GL_FALSE, value_ptr(proj));
}

Camera::~Camera() { }

void Camera::updateAngle(double x, double y, float deltaTime) {
	int xPos = floor(x);
	int yPos = floor(y);

	_horizontalAngle += _mouseSpeed * deltaTime * float(1280 / 2 - xPos);
	_verticalAngle += _mouseSpeed * deltaTime * float(720 / 2 - yPos);

	_direction = vec3(cos(_verticalAngle) * sin(_horizontalAngle),
		sin(_verticalAngle),
		cos(_verticalAngle) * cos(_horizontalAngle));

	_right = vec3(
		sin(_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(_horizontalAngle - 3.14f / 2.0f)
	);

	_up = cross(_right, _direction);
}

void Camera::move(int inputKey, float deltaTime) {
	switch (inputKey) {
	case 0:
		_position += _direction * deltaTime * _speed;
		break;
	case 1:
		_position -= _direction * deltaTime * _speed;
		break;
	case 2:
		_position += _right * deltaTime * _speed;
		break;
	case 3:
		_position -= _right * deltaTime * _speed;
		break;
	}
}

void Camera::updateMatrices() {
	mat4 view = lookAt(_position, _position + _direction, _up);
	glUniformMatrix4fv(_viewLoc, 1, GL_FALSE, value_ptr(view));
}