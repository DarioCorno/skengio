struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float reflectivity;
	int useDiffuseTexture;
	sampler2D difTexture;
	int useSpecularTexture;
	sampler2D specTexture;
	int useCubeMapTexture;
	samplerCube cubeTexture;
};

uniform Material material;