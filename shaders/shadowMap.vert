#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightViewProjMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightViewProjMatrix * model * vec4(aPos, 1.0);
}