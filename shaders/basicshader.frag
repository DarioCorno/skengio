#version 330 core
out vec4 FragColor;

//include material shader source
#include materialuniforms.glsl

//include lights shader sources
#include multiLightUniforms.glsl


uniform vec3 camViewPos;

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;  
in vec4 FragPosLightSpace[NUM_POINT_LIGHTS];    //fragment coords in light space for shadows

void main()
{

    vec3 lresult = vec3(0.0, 0.0, 0.0);
    vec4 finalFrag = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
        if(pointLights[i].enabled == 1) {
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pointLights[i].lightPosition - FragPos);
            vec3 viewDir = normalize(camViewPos - FragPos);

            vec4 lightFrag = CalcPointLight(pointLights[i], norm, FragPos, lightDir, viewDir, texCoord, material);

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


            finalFrag += lightFrag * (1.0 - shadow);

        }
    }

    
    FragColor = finalFrag; 
}
