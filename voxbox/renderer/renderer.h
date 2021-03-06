#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <sstream>
#include "../shaders/shaders.h"
using namespace glm;

class Renderer {
public:
	Renderer(GLFWwindow *window);
	~Renderer();

	void Renderer::render(vec3 position);
	void Renderer::initShaders();
	void Renderer::createArrays();

	GLuint _shaderProgram;
	GLuint _lightProgram;
	GLint _lightPosUniform;

private:
	GLFWwindow *_window;
	GLuint _vbo;
};

