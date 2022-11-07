#version 330 core
out vec4 FragColor;

//sample of including external shaders source
//include singleLightUniforms.glsl
#include multiLightUniforms.glsl

#include materialuniforms.glsl

uniform sampler2D difTexture;
uniform samplerCube cubeTexture;
uniform vec3 camViewPos;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;  
in vec4 FragPosLightSpace[NUM_POINT_LIGHTS];

void main()
{

    vec3 lresult = vec3(0.0, 0.0, 0.0);
    vec4 finalFrag = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
        if(pointLights[i].enabled == 1) {

            // ambient
            vec4 ambient = vec4(pointLights[i].lightAmbient * materialAmbient, 1.0);
  	
            // diffuse calculation
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pointLights[i].lightPosition - FragPos);
            float angle = max(dot(norm, lightDir), 0.0);
            vec4 diffuse = vec4((angle * materialDiffuse) * pointLights[i].lightDiffuse, 1.0);

            //specular
            vec3 viewDir = normalize(camViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
            vec4 specular = vec4(pointLights[i].lightSpecular * (spec * materialSpecular), 1.0);  
    
            //cube reflections
            vec3 refVec = reflect(lightDir, norm);
            vec4 refTex = texture(cubeTexture, refVec) * materialReflectivity;
            vec4 difTex = texture(difTexture, texCoord) * (1.0 - materialReflectivity);

            //shadow
            vec3 projCoords = FragPosLightSpace[i].xyz / FragPosLightSpace[i].w;
            projCoords = projCoords * 0.5 + 0.5;
            float closestDepth = texture(pointLights[i].depthMap, projCoords.xy).r; 
            float currentDepth = projCoords.z;
            float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  
            float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;  



            finalFrag += ((ambient * difTex) + (diffuse * difTex) + (diffuse * refTex) + specular) * (1.0 - shadow);

            //finalFrag += vec4(lresult, 1.0) * (difTex + refTex) * (1.0 - shadow);
        }
    }

    
    FragColor = finalFrag; 
}



//void main()
//{
//    // ambient
//    vec3 ambient = lightDiffuse * materialAmbient;
//  	
//    // diffuse calculation
//
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPosition - FragPos);
//    float angle = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = (angle * materialDiffuse) * lightDiffuse;
//
//    //specular
//    vec3 viewDir = normalize(camViewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
//    vec3 specular = lightDiffuse * (spec * materialSpecular);  
//    
//    vec3 result = (ambient + diffuse + specular);
//
//    //cube reflections
//    vec3 refVec = reflect(lightDir, norm);
//    vec4 refTex = texture(cubeTexture, refVec);
//    vec4 difTex = texture(difTexture, texCoord);
//    
//    FragColor = vec4(result, 1.0) * ((difTex * (1.0 - materialReflectivity)) + (refTex * materialReflectivity)); 
//}
