#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//bind the VAO(vertex array object) first. 

	glBindBuffer(GL_ARRAY_BUFFER, VBO);//set vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);






	//<load and create a texture>
	//////////////////////////////////////////////////////////////////////////////
	unsigned int texture1,texture2;
	//texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); //bind that texture to the currently active texture unit.
	//set the texture wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set the texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load imge & create texture & generate mipmaps
	//----before loading, flip the y-axis
	stbi_set_flip_vertically_on_load(true);
	//----OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, 

	int width, height, nChannels;//number of color channel
	unsigned char* data = stbi_load("bg.jpg", &width, &height, &nChannels, 0);//C:\\Users\\alphacircle\\Downloads\\svt
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		/*parameter
		1: texture target, this operation will generate a texture on the currently bound texture object at the same target.
		2: the mipmap level for which we want to create a texture for if you want to set each mipmap level manually.(we just leave it at the base level,0.)
		3: what kind of format we want to store the texture
		4,5: set the resulting texture's width and height. (I stored those earlier when loading the image so we'll use the corresponding variables.)
		6: should always be 0(some legacy stuff)
		7,8: the format and datatype of the source image.
		9: the actual image data*/
		glGenerateMipmap(GL_TEXTURE_2D);
		/*└ will automatically generate all the required mipmaps for the currently bound texture.
		Once glTexImage2D is called, the currently bound texture object now has the texture image attached to it.
		However, it only has the base-level of the texture image loaded.
		If we want to use mipmaps we have to specify all the different images manually by continually incrementing the second argument.
		The other way is to call glGenerateMipmap() after generatine the texture.*/
	}
	else
	{
		std::cout << "★Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);//free the image memory

	//texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("Vroadway_White_Large.png", &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "★Faild to load texture2" << std::endl;
	}
	stbi_image_free(data);


	myShader.use();//must activate the shader before setting uniforms!
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);//① set manually
	myShader.setInt("texture2", 1);//② or use shader class
	//////////////////////////////////////////////////////////////////////////////






	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		Esc(window);

		//render
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

    /////////////////////////////////////////////////////////////////////////////
		//bind texture on corresponding texture units
		glActiveTexture(GL_TEXTURE0); //active the texture unit first, before binding texture
						//└is always by default activated
						/*They are defined in order so we could also get GL_TEXTURE8 via GL_TEXTURE0 + 8 for example,
						which is useful when we'd have to loop over several texture units.*/
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
						
		//render container
		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    ////////////////////////////////////////////////////////////////////////////

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
