#version 330 core
out vec4 FragColor;

//sample of including external shaders source
//include singleLightUniforms.glsl
#include multiLightUniforms.glsl

#include materialuniforms.glsl

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
            vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
            if(useSpecularTexture == 1) {
                specular = vec4(pointLights[i].lightSpecular * (spec * texture(specTexture, texCoord).r ), 1.0);  
            } else {
                specular = vec4(pointLights[i].lightSpecular * (spec * materialSpecular), 1.0);  
            }
    
            //cube reflections
            vec3 refVec = reflect(lightDir, norm);
            vec4 refTex = texture(cubeTexture, refVec) * materialReflectivity;
            vec4 difTex = texture(difTexture, texCoord) * (1.0 - materialReflectivity);

            //shadows
            float shadow = 0.0;
            if(pointLights[i].castShadows == 1) {
                vec3 projCoords = FragPosLightSpace[i].xyz / FragPosLightSpace[i].w;
                projCoords = projCoords * 0.5 + 0.5;
                //float closestDepth = texture(pointLights[i].depthMap, projCoords.xy).r; 
                float currentDepth = projCoords.z;
                float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  

                //sample 9 pixels in a quad to get a softer shadow
                vec2 texelSize = 1.0 / textureSize(pointLights[i].depthMap, 0);
                for(int x = -1; x <= 1; ++x)
                {
                    for(int y = -1; y <= 1; ++y)
                    {
                        float pcfDepth = texture(pointLights[i].depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
                    }    
                }
                shadow /= 9.0;  //points we sampled -1,1 on x, -1,1 on y (9 pixels) 
            }


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
