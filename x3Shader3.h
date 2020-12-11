#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //include glad to get all the required OpenGL headers

#include <string>
#include <iostream>


class Shader
{
public:
	unsigned int ID; //the shader program ID

	//constructor reads and builds the shader
	Shader() {
		//paramter: the file path of the source code of the vertex and fragment shader respectively
		//			that we can store on dist as simple text files


		//☆1. retrive the vertex/fragment source code from filePath
		const char *vertexShaderCode = "#version 410 core\n"
			"layout (location = 0) in vec3 aPos;\n" //the position variable has attribite position 0
			"layout (location = 1) in vec3 aColor;\n" //the color variable has attribute position 1
			"out vec3 myColor;\n" //output a color to the fragment shader
			"void main(){"
			"	gl_Position = vec4(aPos, 1.0);"
			"	myColor = aColor; }\0"; //set myColor to the input color we got from the vertex data
		const char *fragmentShaderCode = "#version 410 core\n"
			"out vec4 FragColor;\n"
			"in vec3 myColor;\n"
			"void main() {"
			"	FragColor = vec4(myColor, 1.0); }\0";

		//☆2. compile shaders
		unsigned int vertexShader, fragmentShader;

		//vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShader);
		checkCompileError(vertexShader, "VERTEX");
		//fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);
		checkCompileError(fragmentShader, "FRAGMENT");
		
		//shader Program 
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		//delete the shader as they're linked into our program now and no longer necessary
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//use/activate the shader
	void use()//activates the shader program
	{
		glUseProgram(ID);
	}
	//utility uniform functions 
	//check a uniform location and set its value
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, GLint value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}


private://check shader compile/link errors
	void checkCompileError(GLuint shader, std::string type) {
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM") {
			//print compile errors if any
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "★SHADER_COMPILATION_ERROR: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
			else {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "★PROGRAM_LINKING_ERROR: " <<  type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}

	}
};
#endif // !SHADER_H

