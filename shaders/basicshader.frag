#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;  

//sample of including external shaders source
#include singleLightUniforms.glsl

#include materialuniforms.glsl

uniform sampler2D difTexture;
uniform samplerCube cubeTexture;
uniform vec3 viewPos;

//https://github.com/tuxalin/procedural-tileable-shaders
//_include perlin.glsl

void main()
{
    // ambient
    vec3 ambient = lightDiffuse * materialAmbient;
  	
    // diffuse calculation

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float angle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (angle * materialDiffuse) * lightDiffuse;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightDiffuse * (spec * materialSpecular);  
    
    vec3 result = (ambient + diffuse + specular);

    //cube reflections
    vec3 refVec = reflect(lightDir, norm);
    vec4 refTex = texture(cubeTexture, refVec);
    vec4 difTex = texture(difTexture, texCoord);
    
    FragColor = vec4(result, 1.0) * ((difTex * (1.0 - materialReflectivity)) + (refTex * materialReflectivity)); 
}