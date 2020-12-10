#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//When the user resizes the window the viewport shoud be adjusted as well.
//↓this method is called each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//the first two patameter : set the location of the lower left corner of the window
	//the rest : set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
}

//Input control
//process all input : query GLFW whether relevant keys are pressed/released the frame and react accordingly
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
			//↑this function takes the window as input toghther with a key. / returns whether this key is currently being pressed.(or not)
	//check whether the user has presed the ESC key.(if not, returns GLFW_RELEASE)
}





int main() {
	glfwInit();
	//To tell GLFW that OpenGL version(3) we want to use.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//To tell GLFW we want to use the core-propile means we'll get access to a smaller subset of OpenGL features
	//without backwards-compatible features we no longer need.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Xion's OpenGL Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//To make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);
	//When the window is first displayed framebuffer_size_call gets called as well with the resulting window dimension.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//initialize GLAD(←manages function pointers for OpenGl)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD T.T" << std::endl;
		return -1;
	}
	//GLAD functions for load the address of the OpenGL function pointers which is OS-specific.
	//GLFW gives us address(glfwGeoProc--) the defines the correct function based on which OS we're comiling for


	float vertics[] = {
		-0.5f,	-0.5f,	0.0f,
		 0.5f,	-0.5f,	0.0f,
		 0.0f,	 0.5f,	0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertics), vertics, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);



	//render loop ∞
	while (!glfwWindowShouldClose(window)) {
				//↑check at the start of each loop iteration if GLFW has been instructed to close
		
		//check whether the user want to quick 
		processInput(window);

		//render
		glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		//↑swap the color buffer
		//window apply a double buffer for rendering. 
		//the front buffer contain the final output image that is shown at the screen, 
		//while all the rendering commands draw to the back buffer. 
		//As soon as all the rendering commands are finished we swap the back buffer to the front buffer
		//so the image can be displayed without still being rendered to, removing all flickering issues.
		glfwPollEvents();
		//↑check if any events are triggered(like keyboard input or mouse movement), updates the window state, calls functions.
	}

	//As soon as we exit the render loop we have to properly delete all of GLFW's resources that were allocated.
	glfwTerminate();
	return 0;
}
