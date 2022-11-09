struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float reflectivity;
	sampler2D difTexture;
	int useSpecularTexture;
	sampler2D specTexture;
	samplerCube cubeTexture;
};

uniform Material material;