struct LightData {
    int lightType;

    int enabled;
    int castShadows;

    vec3 direction;
    vec3 lightPosition;
    
    float constantAtt;
    float linearAtt;
    float quadraticAtt;
	
    vec3 lightDiffuse;
    vec3 lightAmbient;
    vec3 lightSpecular;

    mat4 lightViewProjMatrix;
    sampler2D depthMap;
    samplerCube depthCubeMap;
};

//struct SpotLight {
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
//  
//    float constant;
//    float linear;
//    float quadratic;
//  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;       
//};

#willdefine NUM_LIGHTS

#ifdef NUM_LIGHTS
#if(NUM_LIGHTS > 0)
uniform LightData lights[NUM_LIGHTS];
#endif
#endif

// calculates the color when using a directional light.
vec4 CalcDirLight(LightData light, vec3 normal, vec3 viewDir, vec2 texCoord, Material material)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse power
    float diff = max(dot(normal, lightDir), 0.0);
    // specular power
    vec3 reflectDir = normalize( reflect(-lightDir, normal) );
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //cubemap reflections
    vec3 refVec = reflect(viewDir, normal);
    refVec.z = -refVec.z;       //cubemaps in ogl have inverted z
    vec4 refTex = vec4(0.0);
    if(material.useCubeMapTexture == 1) {
        refTex = texture(material.cubeTexture, -refVec) * material.reflectivity;
    }

    vec4 difTex = vec4(0.0);
    if(material.useDiffuseTexture == 1) {
        difTex = texture(material.difTexture, texCoord) * (1.0 - material.reflectivity);
    } else {
        difTex = vec4(material.ambient, 1);
    }

    // combine results
    vec4 ambient =  vec4( light.lightAmbient * vec3(refTex + difTex) , 1.0);
    vec4 diffuse =  vec4( light.lightDiffuse * diff * material.diffuse , 1.0);
    vec4 specular;
    if(material.useSpecularTexture == 1) {
        specular = vec4( light.lightSpecular * spec * vec3(texture(material.specTexture, texCoord)), 1.0);
    } else {
        specular = vec4( light.lightSpecular * (spec * material.specular), 1.0);  
    }

    return ( ambient  + specular + diffuse );
}

float CalcDirShadows(LightData light, vec4 FragPosLightSpace, vec3 norm, vec3 lightDir) {
    //shadows
    float shadow = 0.0;
    if(light.castShadows == 1) {
        vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        //float closestDepth = texture(light.depthMap, projCoords.xy).r; 
        float currentDepth = projCoords.z;
        float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  
    
        //sample 9 pixels in a quad to get a softer shadow
        vec2 texelSize = 1.0 / textureSize(light.depthMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(light.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;  //points we sampled -1,1 on x and -1,1 on y (9 pixels) 
    }

    return shadow;
}


// calculates the color when using a point light.
vec4 CalcPointLight(LightData light, vec3 norm, vec3 FragPos, vec3 lightDir, vec3 viewDir, vec2 texCoord, Material material)
{

    // ambient
    //cube reflections
    vec3 refVec = reflect(viewDir, norm);
    refVec.z = -refVec.z;       //cubemaps in ogl have inverted z
    vec4 refTex = vec4(0.0);
    if(material.useCubeMapTexture == 1) {
        refTex = texture(material.cubeTexture, -refVec) * material.reflectivity;
    }

    vec4 difTex = vec4(0.0);
    if(material.useDiffuseTexture == 1) {
        difTex = texture(material.difTexture, texCoord) * (1.0 - material.reflectivity);
    } else {
        difTex = vec4(material.ambient, 1);
    }

    vec4 ambient = vec4(light.lightAmbient * vec3(refTex + difTex), 1.0);
  	
    // diffuse calculation
    float angle = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4((angle * material.diffuse) * light.lightDiffuse, 1.0);

    //specular
    //vec3 viewDir = normalize(camViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    if(material.useSpecularTexture == 1) {
        specular = vec4(light.lightSpecular * (spec * texture(material.specTexture, texCoord).r ), 1.0);  
    } else {
        specular = vec4(light.lightSpecular * (spec * material.specular), 1.0);  
    }
    
    //calc attenuation according to light - fragment distance
    float distance    = length(light.lightPosition - FragPos);
    float attenuation = 1.0 / (light.constantAtt + light.linearAtt * distance + 
    		        light.quadraticAtt * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient  + specular + diffuse );

}

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcPointShadow(LightData light, vec3 viewPos, float far_plane, vec3 FragPos)
{
    float shadow = 0.0;
    if(light.castShadows == 1) {
        // get vector between fragment position and light position
        vec3 fragToLight = FragPos - light.lightPosition;

        float currentDepth = length(fragToLight);

        float bias = 0.15;
        int samples = 20;
        float viewDistance = length(viewPos - FragPos);
        float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
        for(int i = 0; i < samples; ++i)
        {
            float closestDepth = texture(light.depthCubeMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
            closestDepth *= far_plane;   // undo mapping [0;1]
            if(currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
        shadow /= float(samples);
    }
    
    return shadow;
}

//
//// calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // spotlight intensity
//    float theta = dot(lightDir, normalize(-light.direction)); 
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}