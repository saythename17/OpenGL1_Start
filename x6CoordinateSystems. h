/*Coordinate Systems
The vertex shader transform all coordinates to normalized device coordinates(NDC).
NDC:(each vertex's x,y,z should be between -1.0~1.0)--coordinates ourside this range will not be visible.

★Step of Transforming coordinates★

☆1.object(local) coordinate space
: the object's local origin(object begins in this space)
  initial position == (0, 0, 0)
  (The vertices of the container I've been using were specified as coordinates between -0.5~0.5 with 0.0 as its origin.)
      |
[MODEL MATRIX]
      ↓
☆2.world coordinate space
: many other objects also placed to this world's origin
  positioning them inside a larger world.
[VIEW MATRIX] : is a transformation matrix for translate/scales/rotates your object
      ↓         to place it in the world at a location/orientation they belong to.
☆3.view coordinate space
: from the camera or viewer's point of view(==eye space)
  transforming the world-space coordinates to coordinates that are in front of the user's view.
  work with a combination of translations and rotations
      | 
[PROJECTION MATRIX] : 3D coordinate -> NDC(to map in 2D easier)
      ↓
☆4.clip coordinate space
: clip coordinate to the -1.0~1.0 <-the vertex in this range will end up on the screen
  Any coordinate that falls ourside this range is clipped and discarded.
  coordinate are converted to NDC.
  perspective division : the last step to transforms the 4D clip space to 3D NDC.
  (automaticallly at the end of the vertex shader step)
  -orthographic projection
  -perspective projection
<VIEWPORT TRANSFORM>
☆5.screen coordinate spave
: The resulting coordinates are then sent to the rasterizer to turn them into fragment.

*/



#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader() {
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
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//use/activate the shader
	void use()//activates the shader program
	{
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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
					std::cout << "★PROGRAM_LINKING_ERROR: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}

	}
};
#endif // !SHADER_H




