#include "Shader.h"
#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Esc(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void window_size_change(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////// 
const char *vertexShaderCode = "#version 410 core\n"
			"layout (location = 0) in vec3 aPos;\n" //the position variable has attribite position 0
			"layout (location = 1) in vec3 aColor;\n" //the color variable has attribute position 1
			"out vec3 myColor;\n" //output a color to the fragment shader
			"void main(){"
			"	gl_Position = vec4(aPos, 1.0);"
			"	myColor = aColor; }\0"; //set myColor to the input color we got from the vertex data
const char* fragmentShaderCode = "#version 410 core\n"
			"out vec4 FragColor;\n"
			"in vec3 myColor;\n"
			"void main() {" 
			"	FragColor = vec4(myColor, 1.0); }\0"; 
/////////////////////////////////////////////////////////////////////////////////////////////////////







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
	Shader myShader= Shader();



	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//set up vertex data(and buffer) and configure vertex attributes
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 

	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);//bind the VAO(vertex array object) first. 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//set vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	/*parameter
	5 : stride (have to move 6 floats to the right-6 times the size of a float) 
	6 : offset- color attribute starts after the position data (after 3 float values)*/
	glEnableVertexAttribArray(1);
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////






	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		Esc(window);

		//render
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//render a triangle
		myShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
