#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 

out vec4 FragColor;

in vec3 objectColor; // we set this variable in the OpenGL code.
in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{   
    
	// ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    FragColor = vec4(ambient + diffuse + specular, 1.0) 
				* mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4) * vec4(objectColor, 1.0); // 0.4 means 60% first color

};
