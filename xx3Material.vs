		const char* vertexShaderCode = "#version 410 core\n"
			"layout (location = 0) in vec3 aPos;"
			"layout (location = 1) in vec2 aTexCoord;"
			"layout (location = 2) in vec3 aNormal;"
			"out vec2 TexCoord;"
			"out vec3 FragPos;"
			"out vec3 Normal;"
			"uniform mat4 model;"
			"uniform mat4 view;" 
			"uniform mat4 projection;"
			"void main(){"
			//clip V = projection M · view M · model M · object V
			/*Remever that the order of matrix multiplication is reversed(we need to read matrix multiplication from right to left. ←)*/
			"	gl_Position = projection * view * model * vec4(aPos, 1.0);"
			"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
			"   FragPos = vec3(model * vec4(aPos, 1.0));"
			"	Normal = mat3(transpose(inverse(model))) * aNormal;"
			//inverse() is a costly operation for shader(since they have to be done on each vertex of the scene),
			//so try to avoid doing inverse in shader.
			//For an efficient application, we must calculate the normal matrix on the CPU and
			//send it to the shaders via uniform.(like mofel matrix)
			"}\0"; 
