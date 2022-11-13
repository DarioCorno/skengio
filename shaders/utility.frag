#version 330 core
out vec4 FragColor;

uniform vec3 utilityColor;

void main() {
	FragColor = vec4(utilityColor * 2.0, 1.0); 
}