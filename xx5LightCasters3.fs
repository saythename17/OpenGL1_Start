		const char* fragmentShaderCode = "#version 410 core\n"
			"struct Material {"
			"   vec3 ambient;"
			"   vec3 diffuse;"
			"   vec3 specular;"
			"   float shininess;"
			"};"
			"struct Light {"
			"   vec3 position;" 
			"   vec3 direction;"
			"   float cutOff;"
			"   vec3 ambient;"
			"   vec3 diffuse;"
			"   vec3 specular;"
			"   float constant;"
			"   float linear;"
			"   float quadratic;"
			"};"
			"out vec4 FragColor;"
			"in vec2 TexCoord;"
			"in vec3 FragPos;"
			"in vec3 Normal;"
			"uniform sampler2D texture1;"
			"uniform sampler2D texture2;"
			"uniform sampler2D emission;"
			"uniform Material material;"
			"uniform Light light;"
			"uniform vec3 lightPos;"
			"uniform vec3 viewPos;"
			"void main() {"
			"	vec3 lightDir = normalize(lightPos - FragPos);"	
			//check if lighting is inside the soptlight cone
			"   float theta = dot(lightDir, normalize(-light.direction));"
			"	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
			"   vec3 show = step(vec3(1.0), vec3(1.0) - texture(texture2, TexCoord).rgb);"
			"   vec3 emission = texture(emission, TexCoord).rgb * show;"
			"   FragColor +=  vec4(emission, 1.0);"
			/**/
			"	if(theta > light.cutOff) {"
					// ambient
					"   vec3 ambient = light.ambient * material.ambient;"
					// diffuse
					"   vec3 normal = normalize(Normal);"
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
					"   vec3 specular = light.specular * spec * vec3(texture(texture2, TexCoord));"
					// attenuation
					"   float distance = length(light.position - FragPos);"
					"   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));"
					"   diffuse *= attenuation;"
					"   specular *= attenuation;"	
					"   FragColor *= vec4(ambient + diffuse + specular, 1.0);"
			"	}"
			"   else FragColor *= vec4(light.ambient * material.ambient, 1.0);"
			"}\0";
 
