#version 330 core

out vec4 FragColor;
in vec3 objectColor; // we set this variable in the OpenGL code.
in vec2 TexCoord;

uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   // FragColor = vec4(objectColor, 1.0);
   FragColor = vec4(lightColor, 1.0) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4) * vec4(objectColor, 1.0); // 0.4 means 60% first color
   // FragColor = vec4(lightColor * objectColor, 1.0);
};
