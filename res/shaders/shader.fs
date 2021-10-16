#version 330 core

out vec4 FragColor;

in vec3 objectColor; // we set this variable in the OpenGL code.
in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightColor;
uniform vec3 lightPos;  

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   // FragColor = vec4(objectColor, 1.0);
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	FragColor = vec4(ambient + diffuse, 1.0) 
				* mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4) * vec4(objectColor, 1.0); // 0.4 means 60% first color
	
};
