#include "Triangle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}



const char* vertexShaderSource = "#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* vertexShaderSource2 = "#version 410 core\n layout (location = 0) in vec3 aPos;\n void main(){gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);}\0";

const char* fragmentShaderSource = "#version 410 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f,    0.5f,   0.2f,   1.0f);\n"
"}\0";
const char* fragmetShaderSource2 = "#version 410 core\n out vec4 FragColor;\n void main(){FragColor=vec4(1.0f,1.0f,0.0f,1.0f);}\0";




int main() {
    //glfw : initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "XION's OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "☆Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "★Failed to initalize GLAD" << std::endl;
        return -1;
    }

    /*build and compile our shader program
    *
    * vertex shader*/
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //we store the vertex shader as an int and create the shader type as vertex shader

    //attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    /*parameter
    * 1 : shader object to compile
    * 2 : specifiy how many strings we're passing as source code (whice is 1)
    * 3 : actual source code of the vertex shader
    * 4 : ??? */
    glCompileShader(vertexShader);

    int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);

    //Check for shader compoile error
    int success;//if compilation was successful
    char infoLog[512];
    glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
        std::cout << "★ERROR :: SHADER :: VERTEX :: COMPILATION_FAILED★\n" << infoLog << std::endl;
    }

    /*fragment shader*/
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmetShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "★FRAGMENT COMPILATION FAIL★\n" << infoLog << std::endl;
    }


    //link shaders
    /*Link both shader objects into a shader program that we can use for rendering.
    *
    * Shader Program :
    * To use the recently compiled shaders we have to link them to a shader program
    * and then activate this shader program when rendering objects.
    * The activated shader program's shaders will be used when we issue render calls.
    * When linking the shaders into a program
    * it links the outputs of each shader to the inputs of the next shader.
    * If your outputs and inputs do nat match, you'll get error.
    */
    int shaderProgram = glCreateProgram();//returns the ID reference to the newly created program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    //Check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "★ERROR:: SHADER:: PROGRAM:: LINKING_FAILED★\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram2);
    //Delete the shader object once we've linked them into the program; we no linger need them anymore
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);
    //------------------------------ 」input vertex data to the GPU and instructed the GPU how it shoud process the vertex data


    //set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
         // 0.5F,  0.5F, 0.0F,  // TOP RIGHT
         // 0.5F, -0.5F, 0.0F,  // BOTTOM RIGHT
         //-0.5F, -0.5F, 0.0F,  // BOTTOM LEFT
         //-0.5F,  0.5F, 0.0F   // TOP LEFT 

        -1.0f,    -.5f,    .0f,
         -.5f,     .5f,    .0f,
          .0f,    -.5f,    .0f
    };
    float vertices2[] = {
          .0f,    -1.0f,    .0f,
          .5f,     .5f,    .0f,
         1.0f,    -1.0f,    .0f
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle---start from 0
        1, 2, 3   // second Triangle
    };

    //Vertex Array Object(VAO)
    //advantage : when configuring vertex attribute pointers you only have to make those calls once
    //whenever we want to draw the object, we can just bind the corresponding VAO.
    //Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
    //If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.
    /*VAO stores
    Calls glEanbleVertexAttribArray or glDisableVertexAttribArray,
    vertex attribute configurations via glVertexAttribPointer,
    VBO(vertex buffer object)s associated with vertex attributes by calls to glVerteAttribPointer*/
    /*unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);*/

    GLuint VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    

    /*Lingking Vertex Attributes
    * The vertex shader allows use to specigy
    * any input we want in the form of vertex attributes. & great flexibility
    * We have to specify what part of our input data goes to which vertex attribute in the vertes shader.
    * (specify how OpenGL should interpret the vertex data before rendering)
    *
    * Form of verte buffer data
    * --The position data is stored as 32-bit(4 byte) floating point values.
    * --Each position is composed of 3 of those values.
    * --The values are tightly packed in the array. There's no space between each set of 3 values.
    * --The first value is at the begining of the buffer.
    */
    //..::Initialization code (done once)::..
    //※1. bind VAO
    glBindVertexArray(VAOs[0]);
    //※2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);//bind and set vertex buffer, and then configure vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //※3. copy our index array in a element buffer for OpenGL to use
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//copy the index into the buffer
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //※4.then set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    /*parameter
    * 1: which vertex attribute.
    *    we specified the location of position vertex attribute in the vertex shader
    *    with [[[ layout (location = 0) ]]]
    *    set the location of the vertex attribute to 0
    *    since we want to pass data to this vertex attribute, we pass in 0.
    *
    * 2: the size of the vertex attribute.
    *    The vertex attributes is a [[[ vec3 ]]] so if is composed of 3 values.
    *
    * 3: the type of the data
    *    GL_FLOAT (a vec* in GLSL consists of floating point values)
    *
    * 4: whether if we want the data to be normalized
    *    GL_TRUE -- the integer data is normalized to 0(or -1) and 1 when converted to float.
    *
    * 5: stride (the space between consecutive vertex attributes)
    *    Since the next set of position data is located exactly 3 times the size of a float away
    *    we specify that value as the stride.
    *
    * 6: the offset of where the position data begins in the buffer
    */
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //4parameter : stride
    //because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out.
    //glEnableVertexAttribArray(0);//parameter: vertex attribute location--vertex attributes are disabled by default.
    glEnableVertexAttribArray(0);
    //Note : DO NOT unbind the EBO while a VAO is active as the bound EBO is stored in the VAO; keep the EBO bound.

    //Wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw my first triangle
        glUseProgram(shaderProgram2);
        //glBindVertexArray(VAO);
        ////seeing as we only have a single VAO there's no need to bind it every time,
        ////but we'll do so to keep things a bit more organized
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //..::Drawing Code::..
        //※5. draw the object
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //parameter
        //1 : primitive type we would like to draw
        //2 : starting index of vertex array we'd like to draw
        //3 : how many vertices we want to draw
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ////parameter
        ////1 : the mode(primitive type) we want to draw in
        ////2 : the number(count) of elements we'd like to draw.
        ////    (we specified 6 indices so we'll draw 6 vertices in total.)
        ////3 : the type of the indices 
        ////4 : an offset in the EBO(or pass in an index array--when you're not using EBO)
        

        //glfw : swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //optional : de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAOs[0]);
    glDeleteBuffers(1, &VBOs[0]);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    //glfw : terminate, clear all previously allocated. GLFW resources
    glfwTerminate();
    return 0;
}
