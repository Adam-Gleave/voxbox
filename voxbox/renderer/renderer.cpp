#include "../chunks/chunkmanager.h"
using namespace glm;

ChunkManager *chunkManager;

Renderer::Renderer(GLFWwindow *window) {
	//Make new chunk manager object
	chunkManager = new ChunkManager();
	_window = window;
}

Renderer::~Renderer() {
	glDeleteProgram(_shaderProgram);
}

void Renderer::render(vec3 position) {
	chunkManager->render(this, position);
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

	vec3 lightColor = vec3(1.0, 0.92, 0.85);
	GLint lightUniform = glGetUniformLocation(_shaderProgram, "lightColor");
	glUniform3fv(lightUniform, 1, value_ptr(lightColor));

	vec3 lightDir = vec3(0.4, -1.0, 0.3);
	GLint lightDirUniform = glGetUniformLocation(_shaderProgram, "lightDir");
	glUniform3fv(lightDirUniform, 1, value_ptr(lightDir));
}

void Renderer::createArrays() {
	//Create vertex buffer
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo); //Make buffer active object
}