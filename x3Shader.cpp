
/*
---------------------GLSL----------------------------------
#version version_number
in type in_variable_name; //vertex attribute
in type in_variable_name; //vertex attribute2

out type out_variable_name;
  
uniform type uniform_name;
  
void main()
{
  // process input->graphic work->output processed value
  out_variable_name = weird_stuff_we_processed;
}



............................................................
<swizzling>
ex)
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);

<uniform>
global varialble that can be accessed from any shader at any stage in the shader program
............................................................



<Input and Output>
//Vertex shader---sending shader
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0//==can set via glGetAttribLocation
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // directly give a vec3 to vec4's constructor's parameter
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output color variable 
}

//Fragment shader---receiving shader
#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)

void main()
{
    FragColor = vertexColor;
} 
------------------------------------------------------------


//Check maximum number of vertex attributes (limited by the hardware)
//(usually)OpenGL guarantees there are always at least 16 4-component vertex attribute available
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
*/












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

 
const char *vertexShaderCode = "#version 410 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main(){gl_Position = vec4(aPos, 1.0);}\0";
const char* fragmentShaderCode = "#version 410 core\n"
			"out vec4 FragColor;\n"
			"uniform vec4 changeColor;\n"
			"void main() { FragColor = changeColor; }\0"; 







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



	//build and compile the shader program
	//vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	//check for shader compile error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "★ERROR:SHADER:VERTEX:COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	//check for shader compile error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "★ERROR SHADER FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
	}
	//link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check for linking error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "★ERROR SHADER PROGRAM LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);






	//set up vertex data(and buffer) and configure vertex attributes
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 

	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);//bind the VAO(vertex array object) first. 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//set vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//configure vertex attribute
	glEnableVertexAttribArray(0);

	/*I can unbind the VAO afterwards so ther VAO calls won't accidentally modify this VAO,
	but this rarely happens. 
	Modifying other VAOs requires a call to glBindVertesArray anyways so we generally don't unbind VAOs (nor VBOs)
	when it's not directly necessary.*/
	//glBindVertexArray(VAO);






	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		Esc(window);

		//render
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		///////////////////////////////////////////////////////////////
		//be sure to activate the shader before any calls to glUniform
		glUseProgram(shaderProgram);
		//p.s : finding the uniform location does not reaquire you to use shader program(by glUseProgram) first, 
		//		but updating a uniform does require you to first calling glUseProgram.

		
		
		//update shader uniform
		float time = glfwGetTime();//first , retrieve the running time in seconds
		float colorValue = sin(time) / 2.0f + .5f;//vary the color in the range 0.0 ~ 1.0
		//
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "changeColor");//bring the location of the changeColor uniform 
		//parameter2 : the name of the uniform that we want to retrieve the location
		//※※  if this function returns -1, it could not find the location.
		//
		glUniform4f(vertexColorLocation, .0f, colorValue, .0f, 1.0f);//set the uniform value
		//parameter1 : the location of the uniform variable to be modified



		//render a triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfw : swap buffer and poll event (key pressed/release, mouse moved etc..)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	



	//optional - de-allocate all resource once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	//glfw: terminate, clearing all previously allocatedd GLFW resources
	glfwTerminate();
	return 0;
}
