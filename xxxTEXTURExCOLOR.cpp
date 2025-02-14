#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include <cmath>

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void user_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void window_size_change(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//to calculate the pitch and yaw values from mouse-movement events
void mouse_callback(GLFWwindow* window, /*mouse position*/double xPos, double yPos) {
	/*To check if this is the first time we receive mouse input.
	(whenever the window first receives focus of the mouse cursor, happen large sudden jump
	This is often a position that is significantly far away from the center of the screen,
	resulting in large offsets and thus a large movement jump.)*/
	if (firstMouse) {
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = false;
	}

	//calculate the distance of movement between the last and current frame.
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; //reverse (∵ y-coordinates : ↑)
	lastX = (float)xPos;
	lastY = (float)yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}




int main() {
	//initialize and configue
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(1600, 1200, "Xion's OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "★Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, window_size_change);

	//register this function with GLFW each time the mouse moves
	//glfwSetCursorPosCallback(window, mouse_callback);
	//to zoon in, use mouse's scroll wheel
	glfwSetScrollCallback(window, scroll_callback);

	//to calculate the yaw and pitch values from mouse movement.
	//When the application has focus, the mouse cursor should be hiden and stays within the window
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glad : load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	//shader program
	Shader myShader = Shader();

	//set up vertex data(and buffer) and configure vertex attributes
	float vertices[] = {
		// positions          // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int VBO;
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(lightVAO);
	//we onlly need to bind to the VBO, the container's VBO's data already contains the exact data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);







	//load and create textures
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h flip loaded texture's on the y-axis.

	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.	
	unsigned char* data = stbi_load("rose.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Alpha.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "★Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);

	//activate shader
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	//myShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	myShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		



	//------------------------------------------------------
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//per-frame time logic
		//per-frame time logic
		deltaTime = (float)glfwGetTime() - lastFrame;
		lastFrame = (float)glfwGetTime();

		//input
		user_input(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//└also clear the depth buffer(otherwise the depth infomation of the precious frame stays in the buffer)

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		float color = (float)cos(glfwGetTime() / 1.0f) +.5f;
		float color2 = (float)sin(glfwGetTime() / 1.0f) +.5f;
		float color3 = (float)cos(glfwGetTime() / 4.0f) +.5f;
		myShader.setVec3("objectColor", color + 0.7f, color3 + .5f, color2 + .5f);

		//pass projection matrix to shader(in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)4 / (float)3, .1f, 100.0f);
		myShader.setMat4("projection", projection);
		// create a frustum that defines the visible space
		/*frustum : in clip space, viewing range will end up on the user's screen. .
					anything out side the frustum will not end up and will become clipped*/
					//projection = glm::perspective(glm::radians(40.0f), 6.0f / 8.0f, 0.1f, 100.0f); 
					/*parametaer
					1: fov(field of view) value == how large the viewspace is.
					2: aspect ratio == viewport's width / viewport's height
					3,4: distance between near plane and far plane (usually set near distance: 0.1, far distance: 100.0)
						 All the vertices between the near and far plane and inside the frustum  will be rendered.
					* The projection matrix not only maps a given frustum range to clip space,
					* but also manipulates the w value of each vertex coordinate.
					* Each component of the vertex coordinate is divided by its w component.
					* The further away a vertex from the viewer, will receive smaller vertex coordinates.
					*/

					//camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		myShader.setMat4("view", view);

		glBindVertexArray(lightVAO);
		glm::mat4 model = glm::mat4(1.0f);
		
		//move from starting point
		//model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));

		float angle = (float)glfwGetTime() * 40.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(.7f, 0.8f, 0.5f));
		unsigned int modelLocation = glGetUniformLocation(myShader.ID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glfw : swap buffer and poll event (key pressed/release, mouse moved etc..)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	//optional - de-allocate all resource once they've outlived their purpose
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	//glfw: terminate, clearing all previously allocatedd GLFW resources
	glfwTerminate();
	return 0;
}
