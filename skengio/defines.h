#ifndef SK_DEFINES_
#define SK_DEFINES_

//SOME MATH DEFINES
#define M_PI	3.14159
#define D_TO_R     (M_PI/180.0)

//UNIFORM NAMES DEFINES
#define PROJ_MATRIX_UNIFORM_NAME "projMatrix"
#define VIEW_MATRIX_UNIFORM_NAME "viewMatrix"
#define MODEL_MATRIX_UNIFORM_NAME "modelMatrix"

//LIGHT UNIFORM DEFINES
#define LIGHT_POSITION_UNIFORM_NAME "lightPosition"
#define LIGHT_DIFFUSE_UNIFORM_NAME "lightDiffuse"
#define LIGHT_VIEWPROJMATRIX_UNIFORM_NAME "lightViewProjMatrix"

//TEXTURE DEFINES
#define TEXTURE_DIFFUSE_UNIFORM_NAME	"material.difTexture"
#define TEXTURE_CUBEMAP_UNIFORM_NAME	"material.cubeTexture"
#define TEXTURE_DEPTH_UNIFORM_NAME		"material.depthTexture"
#define TEXTURE_SPECULAR_UNIFORM_NAME	"material.specTexture"
#define TEXTURE_USE_SPECULAR_UNIFORM_NAME "material.useSpecularTexture"
#define TEXTURE_USE_DIFFUSE_UNIFORM_NAME "material.useDiffuseTexture"
#define TEXTURE_USE_CUBEMAP_UNIFORM_NAME "material.useCubeMapTexture"

//MATERIAL UNIFORM DEFINES
#define MATERIAL_AMBIENT_UNIFORM_NAME "material.ambient"
#define MATERIAL_DIFFUSE_UNIFORM_NAME "material.diffuse"
#define MATERIAL_SPECULAR_UNIFORM_NAME "material.specular"
#define MATERIAL_SHININESS_UNIFORM_NAME "material.shininess"
#define MATERIAL_REFLECTIVITY_UNIFORM_NAME "material.reflectivity"

//CAMERA UNIFORM DEFINES
#define CAMERA_VIEWPOS_UNIFORM_NAME "camViewPos"
#define CAMERA_TARGETPOS_UNIFORM_NAME "camTargetPos"
#define CAMERA_NEAR_PLANE_UNIFORM_NAME "nearPlane"
#define CAMERA_FAR_PLANE_UNIFORM_NAME "farPlane"
#define CAMERA_FOV_UNIFORM_NAME "camFOV"

#define SCENE_DEFAULT_START 0.0f
#define SCENE_DEFAULT_END 60.0f

#define GUI_SHOW_FPS true
#define GUI_SHOW_LOG true


#endif
