#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "filesystem.h"

//setting
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

//Camera
Camera camera(glm::vec3(.0f, .0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = .0f;
float lastFrame = .0f;





//glfw: whenever the window size changed (by OS or user resize)
void window_size_changed(GLFWwindow* window, int width, int height) {
	//make sure the viewport mathes the new window dimensions;
	//width and height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//process all input: quety GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.processKeyboard(RIGHT, deltaTime);
}

//glfw: whenever the mouse moves
void mouse_move(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; //since the y-coordinates is reversed

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw:whenever the mouse wheel scrolls
void scroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}









int main()
{
	//glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Xion's OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, window_size_changed);
	glfwSetCursorPosCallback(window, mouse_move);
	glfwSetScrollCallback(window, scroll);

	//tell GLFW to capture my mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//tell "stb_image.h" to flip loaded texture's on the y-axis (before loading model)
	stbi_set_flip_vertically_on_load(true);

	//configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	//build shader
	Shader shader;

	//load models
	Model xModel("backpack/backpack.obj");

	//draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//------------------------------------------
	//render loop
	while (!glfwWindowShouldClose(window)) {
		//per-frame time logic
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();

		//input
		processInput(window);

		//render
		glClearColor(.3f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//enable shader before setting uniforms
		shader.use();

		//view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, .1f, 100.0f);;
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		//render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(.0f, .0f, .0f)); //translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  //it's a bit too big for our scene, so scale it down
		shader.setMat4("model", model);
		xModel.Draw(shader);

		//glfw: swap buffers and poll IO events (key pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glfw: terminate, clearing all precviously allocated GLFW resources
	glfwTerminate();
	return 0;
}

