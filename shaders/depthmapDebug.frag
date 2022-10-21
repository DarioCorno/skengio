#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthTexture;
uniform float camNearPlane;
uniform float camFarPlane;

float near_plane = camNearPlane;
float far_plane = camFarPlane;

//include the linearize depth function
#include LinearizeDepth.glsl

void main()
{             
    float depthValue = texture(depthTexture, TexCoords).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue, near_plane, far_plane) / far_plane), 1.0); // perspective
    //FragColor = vec4(vec3(1.0, 1.0, 0.0) * vec3(depthValue), 1.0); // orthographic
}