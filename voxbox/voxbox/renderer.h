#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sstream>
#include "shaders.h"

class Renderer
{
public:
	Renderer(GLFWwindow *window);
	~Renderer();

	void Renderer::render();
	void Renderer::initShaders();
	void Renderer::createArrays();

private:
	GLFWwindow *_window;
	GLuint _shaderProgram;
	GLuint _vbo;
};

