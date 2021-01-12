		const char* fragmentShaderCode = "#version 410 core\n"
			"struct Material {"
			"   vec3 ambient;"
			"   vec3 diffuse;"
			"   vec3 specular;"
			"   float shininess;"
			"};"
			"struct Light {"
			"   vec3 position;"
			"   vec3 ambient;"
			"   vec3 diffuse;"
			"   vec3 specular;"
			"};"
			"out vec4 FragColor;"
			"in vec2 TexCoord;"
			"in vec3 FragPos;"
			"in vec3 Normal;"
			"uniform sampler2D texture1;"
			"uniform sampler2D texture2;"
			"uniform Material material;"
			"uniform Light light;"
			"uniform vec3 lightPos;"
			"uniform vec3 viewPos;"
			"void main() {"
			"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
			// ambient
			"   vec3 ambient = light.ambient * material.ambient;"
			// diffuse
			"   vec3 normal = normalize(Normal);"
			"	vec3 lightDir = normalize(lightPos - FragPos);"
			"	float diff = max(dot(normal, lightDir), 0.0);"
			"   vec3 diffuse = light.diffuse * (diff * material.diffuse);"
			//specular
			"   vec3 viewDir = normalize(viewPos - FragPos);"
			"   vec3 reflectDir = reflect(-lightDir, normal);"
			/*-lightDir : we reverse its direction to get the correct reflect vector 
			* The reflect function expects the first vector to point from the light 
			* towards the fragment's position.(This depends on the order of subtraction earlier on
			* when we calculated the lightDir vector)*/
			"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);"
			"   vec3 specular = (material.specular * spec) * light.specular;"
			"   vec3 result = ambient + diffuse + specular;"
			"   FragColor *= vec4(result, 1.0);"
			"}\0";
		
