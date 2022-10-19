#ifndef _SK_DEFINES_
#define _SK_DEFINES_

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

//TEXTURE DEFINES
#define TEXTURE_DIFFUSE_UNIFORM_NAME	"difTexture"
#define TEXTURE_CUBEMAP_UNIFORM_NAME	"cubeTexture"

//MATERIAL UNIFORM DEFINES
#define MATERIAL_AMBIENT_UNIFORM_NAME "materialAmbient"
#define MATERIAL_DIFFUSE_UNIFORM_NAME "materialDiffuse"
#define MATERIAL_SPECULAR_UNIFORM_NAME "materialSpecular"
#define MATERIAL_SHININESS_UNIFORM_NAME "materialShininess"
#define MATERIAL_REFLECTIVITY_UNIFORM_NAME "materialReflectivity"

//CAMERA UNIFORM DEFINES
#define CAMERA_VIEWPOS_UNIFORM_NAME "viewPos"

#define SCENE_DEFAULT_START 0.0f
#define SCENE_DEFAULT_END 60.0f

#define GUI_SHOW_FPS true
#define GUI_SHOW_LOG true


#endif
