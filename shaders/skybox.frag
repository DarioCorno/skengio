#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

struct CubeMapMaterial {
    samplerCube cubeTexture;
};

uniform CubeMapMaterial material;

void main()
{    
    FragColor = texture(material.cubeTexture, TexCoords);
}