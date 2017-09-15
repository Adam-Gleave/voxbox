#include <list>
#include "../chunks/superchunk.h"
using namespace std;
using namespace glm;

vec3 _lightPos;
SuperChunk *_superChunk;

Renderer::Renderer(GLFWwindow *window) {
	//Set global light position
	_lightPos = vec3(18, 50, 28);
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

	mat4 model = mat4(1.0f);
	GLint modelUniform = glGetUniformLocation(_shaderProgram, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(model));

	mat4 view = lookAt(vec3(36, 36, 36), vec3(0, 0, 0), vec3(0, 1, 0));;
	GLint viewUniform = glGetUniformLocation(_shaderProgram, "view");
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, value_ptr(view));

	mat4 proj = perspective(radians(45.0f), 800.0f / 600.0f, 1.0f, 80.0f);
	GLint projUniform = glGetUniformLocation(_shaderProgram, "proj");
	glUniformMatrix4fv(projUniform, 1, GL_FALSE, value_ptr(proj));

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