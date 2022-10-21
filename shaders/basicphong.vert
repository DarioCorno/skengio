#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 ourColor;
out vec3 Normal;
out vec3 FragPos;

void main()
{
   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
   ourColor = aColor;
   Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
   FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
}
