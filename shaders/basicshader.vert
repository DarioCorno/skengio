#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aText;

#willdefine NUM_POINT_LIGHTS

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightViewProjMatrix[NUM_POINT_LIGHTS];

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace[NUM_POINT_LIGHTS];

void main()
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
	texCoord = aText;
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
	FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
	for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
		FragPosLightSpace[i] = lightViewProjMatrix[i] * vec4(FragPos, 1.0);
	}
}
