#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(GLint modelLoc, GLint viewLoc, GLint projLoc, GLint cameraLoc);
	~Camera();
	void updateAngle(double x, double y, float deltaTime);
	void move(int inputKey, float deltaTime);
	void updateMatrices();

private:
	GLint _modelLoc;
	GLint _viewLoc;
	GLint _projLoc;
	GLint _cameraLoc;

	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _up;
	glm::vec3 _right;
	float _horizontalAngle;
	float _verticalAngle;
	float _speed;
	float _mouseSpeed;
};
