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
			"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3); }\0";
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

void Esc(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
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
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	unsigned char* data = stbi_load("bg.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
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

	data = stbi_load("Logo.png", &width, &height, &nrChannels, 0);
	if (data)
	{
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


	glEnable(GL_DEPTH_TEST);
	/*OpeGL stores depth information in a buffer called the z-buffer that allows OpenGL to decide when to draw over a pixel and when not to.
	* z-buffer == depth buffer
	* The depth is stored whithin each fragment('s z value).
	* If the current fragment is befind the other fragment, it is discarded, otherwise overwritten.(compares its depth values with the z-buffer)
	* This process is depth testing.--->default : disable
	(Some side being drawn over of other sides this happens because
	when OpenGL draws your object by triangle,
	it will overwrite any pixel color that may have already been drawn there before.)*/


	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		Esc(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //also clear the depth buffer(otherwise the depth infomation of the precious frame stays in the buffer)

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//activate shader
		myShader.use();





		/////////////////////////////////////////////////////////////
		/// <Coordinate-System> /////////////////////////////////////
		//create the transformation matrices we should pass them to our shaders.
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime()* 2.0f * glm::radians(-55.0f)
			, glm::vec3(-.5f, -1.0f, .5f));
		
		//transforming the vertex coordinates to world coordinates (By multiplying the vertex coordinates with this model matrix)
		glm::mat4 view = glm::mat4(1.0f);
		//note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.5f));
		/*To move a camera backwards == To move the entire scene forward
		* OpenGL is a right-handed syste. So if we wnat to move backwards, 
		* we have to move in the positive z-axis. == translate the scene towards the negative z-axis.
		* (move the entire scene aroudn inversed to where we want to the camera to move)
		* This gives the impression that we are moving backwards.
		*/

		glm::mat4 projection = glm::mat4(1.0f);
		// create a frustum that defines the visible space
		/*frustum : in clip space, viewing range will end up on the user's screen. .
					anything out side the frustum will not end up and will become clipped*/
		projection = glm::perspective(glm::radians(90.0f), 8.0f / 7.0f, 0.1f, 100.0f); 
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

		//send the matrices to the shader
		unsigned int modelLocation = glGetUniformLocation(myShader.ID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		unsigned int viewLocation = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		unsigned int projectionLocation = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
		/////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////
		
		
		
		
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glfw : swap buffer and poll event (key pressed/release, mouse moved etc..)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	//optional - de-allocate all resource once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//glfw: terminate, clearing all previously allocatedd GLFW resources
	glfwTerminate();
	return 0;
}
