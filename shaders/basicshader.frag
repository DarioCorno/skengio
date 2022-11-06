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

            vec3 projCoords = FragPosLightSpace[i].xyz / FragPosLightSpace[i].w;
            projCoords = projCoords * 0.5 + 0.5;
            float closestDepth = texture(pointLights[i].depthMap, projCoords.xy).r; 
            float currentDepth = projCoords.z;
            float inShadow = currentDepth > closestDepth ? 1.0 : 0.0;  

            // ambient
            vec3 ambient = pointLights[i].lightDiffuse * materialAmbient;
  	
            // diffuse calculation
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pointLights[i].lightPosition - FragPos);
            float angle = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = (angle * materialDiffuse) * pointLights[i].lightDiffuse;

            //specular
            vec3 viewDir = normalize(camViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
            vec3 specular = pointLights[i].lightDiffuse * (spec * materialSpecular);  
    
            if(inShadow > 0) {
                diffuse = vec3(0.0);
                specular = vec3(0.0);
                ambient = vec3(0.0);
            }

            lresult += (ambient * ( diffuse + specular));

            //cube reflections
            vec3 refVec = reflect(lightDir, norm);
            vec4 refTex = texture(cubeTexture, refVec);
            vec4 difTex = texture(difTexture, texCoord);

            finalFrag += vec4(lresult, 1.0) * ((difTex * (1.0 - materialReflectivity)) + (refTex * materialReflectivity));
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
