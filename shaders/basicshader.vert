#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aText;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightViewProjMatrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main()
{
   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
   texCoord = aText;
   Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
   FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
}
