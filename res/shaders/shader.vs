#version 330 core

layout (location = 0) in vec3 aPos;			// the position variable has attribute position 0
layout (location = 1) in vec3 aColor;		// the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;	// the tex coord variable has attribute position 2
layout (location = 3) in vec3 aNormal;		// the tex coord variable has attribute position 3

uniform mat4 transform;	//  for transformation

uniform mat4 model;			// model to world
uniform mat4 view;			// world to camera
uniform mat4 projection;	// camera to screen

out vec2 TexCoord;	// texture coordinate
out vec3 objectColor;	// output a color to the fragment shader
out vec3 FragPos;  
out vec3 Normal;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   objectColor = vec3(aColor.x, aColor.y, aColor.z);   // set ourColor to the input color we got from the vertex data
   TexCoord = aTexCoord;
   FragPos = vec3(model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(model))) * aNormal;  
};

