#include <list>
#include "../chunks/superchunk.h"
using namespace std;
using namespace glm;

vec3 _lightPos;
SuperChunk *_superChunk;

Renderer::Renderer(GLFWwindow *window) {
	//Set global light position
	_lightPos = vec3(128, 150, 128);
	//Make new SuperChunk object
	_superChunk = new SuperChunk();
	_window = window;
}

Renderer::~Renderer() { 
	glDeleteProgram(_shaderProgram);
}

void Renderer::render() {
	_superChunk->render(this);

	//Update buffers and register any events
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void Renderer::initShaders() {
	const char *vertSource = Shaders::vertShader();
	const char *fragSource = Shaders::fragShader();

	//Compile shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	//Link vertex and fragment shaders to shader program
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glBindFragDataLocation(_shaderProgram, 0, "outColor");
	glLinkProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	GLint lightUniform = glGetUniformLocation(_shaderProgram, "lightColor");
	glUniform3fv(lightUniform, 1, value_ptr(lightColor));

	GLint lightPosUniform = glGetUniformLocation(_shaderProgram, "lightPos");
	glUniform3fv(lightPosUniform, 1, value_ptr(_lightPos));
}

void Renderer::createArrays() {
	//Create vertex buffer
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo); //Make buffer active object
}