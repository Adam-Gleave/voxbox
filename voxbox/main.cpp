#include "renderer/renderer.h"
#include "renderer/camera.h"

GLFWwindow *initWindow() {
	//Support OpenGL 3.2 at least
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create resizable GLFW window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL", nullptr, nullptr);

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

	//Set up shader program and initialise camera
	renderer->initShaders();
	GLint modelUniform = glGetUniformLocation(renderer->_shaderProgram, "model");
	GLint viewUniform = glGetUniformLocation(renderer->_shaderProgram, "view");
	GLint projUniform = glGetUniformLocation(renderer->_shaderProgram, "proj");
	Camera *camera = new Camera(modelUniform, viewUniform, projUniform);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double currentTime = glfwGetTime();
	float lastTime;
	float deltaTime;

	//Main loop
	//Render scene, exit on escape key press
	while (!glfwWindowShouldClose(window)) {
		lastTime = currentTime;
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		//Get co-ordinates of camera;
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);
		camera->updateAngle(xPos, yPos, deltaTime);

		//Clear background to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->render();

		//Determine keyboard inputs
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera->move(0, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera->move(1, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera->move(2, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera->move(3, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			delete renderer;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		camera->updateMatrices();
	}

	glfwTerminate();

	return 0;
}