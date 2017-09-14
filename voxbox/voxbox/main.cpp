#include "renderer/renderer.h"

GLFWwindow *initWindow() {
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

int main() {
	glfwInit();

	GLFWwindow *window = initWindow();
	glfwMakeContextCurrent(window);

	//Set up GLEW with Modern GL
	glewExperimental = GL_TRUE;
	glewInit();

	Renderer *renderer = new Renderer(window);

	//Functions for array and shader creation
	renderer->createArrays();
	renderer->initShaders();

	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	//Main loop
	//Render scene, exit on escape key press
	while (!glfwWindowShouldClose(window)) {
		//Clear background to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->render();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			delete renderer;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwTerminate();

	return 0;
}