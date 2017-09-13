#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shaders.h"
using namespace std;
using namespace glm;

GLuint shaderProgram;

//Vertices to draw
//Left: co-ords, right: colors for frag shader
GLfloat verts[] = {
	//front
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f, //bottom left
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f, //top left
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f, //bottom right
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f, //top right

	//back
	-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f, //bottom left
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f, //top left
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, //bottom right
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f  //top right
};

//Elements for EBO
GLuint elements[] = {
	//front
	0, 1, 2,
	3, 2, 1,

	//base
	0, 4, 2,
	6, 2, 4,

	//back
	4, 5, 6,
	7, 6, 5,

	//right
	2, 3, 6,
	7, 6, 3,

	//left
	0, 1, 4,
	5, 4, 1,

	//top
	1, 3, 5,
	7, 5, 3,
};

void render(GLFWwindow *window) 
{
	//Clear background to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Update buffers and register any events
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow *initWindow() 
{
	//Support OpenGL 3.2 at least
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create resizable GLFW window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

	return window;
}

void initShaders() 
{
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
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//Get pointers to attributes
	GLint positionAttrib = glGetAttribLocation(shaderProgram, "coord3d");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));

	mat4 view = lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0));;
	GLint viewUniform = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, value_ptr(view));

	mat4 proj = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);
	GLint projUniform = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(projUniform, 1, GL_FALSE, value_ptr(proj));
}

void createArrays() 
{
	//Create vertex array
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Create vertex buffer and populate with vertices
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //Make buffer active object
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	//Create element buffer and populate with indices
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void freeResources() 
{
	glDeleteProgram(shaderProgram);
}

int main() 
{
	glfwInit();

	GLFWwindow *window = initWindow();
	glfwMakeContextCurrent(window);

	//Set up GLEW with Modern GL
	glewExperimental = GL_TRUE;
	glewInit();

	//Functions for array and shader creation
	createArrays();
	initShaders();

	glEnable(GL_DEPTH_TEST);

	//Main loop
	//Render scene, exit on escape key press
	while (!glfwWindowShouldClose(window)) 
	{
		render(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			freeResources();
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwTerminate();

	return 0;
}