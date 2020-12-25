/*
		const char* vertexShaderCode = "#version 410 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec2 aTexCoord;\n"
			"out vec2 TexCoord;\n"
			"uniform mat4 model;"
			"uniform mat4 view;"
			"uniform mat4 projection;"
			"void main(){"
			//clip V = projection M · view M · model M · object V
			/*Remever that the order of matrix multiplication is reversed(we need to read matrix multiplication from right to left. ←)*/
			"	gl_Position = projection * view * model * vec4(aPos, 1.0);"
			"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);}\0"; 
		const char* fragmentShaderCode = "#version 410 core\n"
			"out vec4 FragColor;\n"
			"in vec2 TexCoord;"
			"uniform sampler2D texture1;"
			"uniform sampler2D texture2;"
			"void main() {"
			"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.6); }\0";
*/


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//camera
glm::vec3 cameraPosition = glm::vec3(.0f,  .0f,  3.0f);
glm::vec3 cameraFront	 = glm::vec3(.0f,  .0f, -1.0f); // -z axis
glm::vec3 cameraUp		 = glm::vec3(.0f, 1.0f,   .0f);
/* timing
*Graphics applications usually keep track of deltatime that stores the time it took to render the last frame.
* to make sure it runs the same on all kinds of hardware.
* Because many machines have different processing powers and the result of that is that some are able to render much more frames than others.*/ 
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; 

void user_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 5.5f * deltaTime;

	//position vector - direction vector
	//-1.0f(-z axis) → += : move forward(-) | -= : move backward(--)==(+)
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront * 2.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront * 2.0f;

	//cross product to create right vector(+x axis)
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//nomalize the resulting right vector.-->resulting is a consistent movement speed.
	//If I would not normalize this vector, the resulting may return differently sized vectors
	//based on the cameraFront variable.
}

void window_size_change(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
	
	//world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,   0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3( 1.3f, -2.0f,  -2.5f),
		glm::vec3( 1.5f,  2.0f,  -2.5f),
		glm::vec3( 1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

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

	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);

	//pass projection matrix to shader before render loop
	//(as projection matrix rarely changes -> there's no need to do this per frame)
	glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)6 / (float)8, 0.1f, 100.0f);
	myShader.setMat4("projection", projection);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
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

		//activate shader
		myShader.use();


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/*//★Get the camera position		
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		//z-axis is going through your screen towards you 
		//so if you want to the camera to move backwards, we move along the positive z-aixs.
		//note the position vector is inverted since we want to move the world in opposite direction of where the camera to move.

		//★Get the camera direction 
		// = camera position vectorm - scene's origin vector
		glm::vec3 cameraTarget = glm::vec3(.0f, .0f, .0f);// let the camera point to the origin of the screen
		glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		//└It is actually pointing in the reverse direction of what if is targeting

		//★Get the right vector 
		//└ that represents the positive x-axis of the camera space.
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//points upwards in world space
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//cross product up(+y-axis) × direction(+z-axis) = positive x-axis's direction
		// p.s)) direction × up = negative x-axis

		//★Get the up axis = right(+x) × direction(+z)
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		glm::mat4 view=glm::lookAt(glm::vec3(.0f, .0f,3.0f),//position
								   glm::vec3(.0f, .0f, .0f),//target
								   glm::vec3(.0f,1.0f, .0f));//up 
		//LookAt matrix - transfroms all the world coordinates to the view space we defined.
		//				- makes view matrix looks at a given target
		/*To move a camera backwards == To move the entire scene forward
		* OpenGL is a right-handed syste. So if we want to move backwards, 
		* we have to move in the positive z-axis. == translate the scene towards the negative z-axis.
		* (move the entire scene around inversed to where we want to the camera to move)
		* This gives the impression that we are moving backwards.*/

		glm:: mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		myShader.setMat4("view", view);
		////////////////////////////////////////////////////////////////////////////////////////////////////

		//glm::mat4 projection = glm::mat4(1.0f);
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
		//myShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = angle = (float)glfwGetTime() * 20.0f;
			if (i % 3 == 0) {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(.7f + i / angle * 20, angle / 20 * 0.3f - 0.5f, 0.5f + i / 10));
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(.7f + i, 0.3f - 0.5f, 0.5f + i));
			unsigned int modelLocation = glGetUniformLocation(myShader.ID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glfw : swap buffer and poll event (key pressed/release, mouse moved etc..)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	//optional - de-allocate all resource once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glfw: terminate, clearing all previously allocatedd GLFW resources
	glfwTerminate();
	return 0;
}
