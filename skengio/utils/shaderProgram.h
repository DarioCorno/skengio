#ifndef SHADER_H
#define SHADER_H

#include "../logger.h"

#include <string>
#include <vector>

#define GLEW_STATIC
#include "GLEW/glew.h"
#include "skengio/camera.h"

namespace SKEngio {

    struct ShaderDefine {
        std::string defineName = "";
        std::string defineValue = "";
    };

    //a simple class to load and compile shaders
    class ShaderProgram
    {
        public:
        
            enum SHADERTYPE {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION};

            unsigned int shaders[4]{};    //shaders IDs for every shader type
            unsigned int programID = 0;

            ShaderProgram() = default;

            // prevent copying object
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram(ShaderProgram&&) = delete;
            ShaderProgram& operator=(const ShaderProgram&) = delete;
            ShaderProgram& operator=(ShaderProgram&&) = delete;

            ~ShaderProgram();

            void LoadShader(const std::string& strPath, const std::string& strFileName, SHADERTYPE typeShader);
            void LoadShader(const std::string& strPath, const std::string& strFileName, SHADERTYPE typeShader, std::list<ShaderDefine> defines);
            void OnDestroy();
            void CreateProgram();

            void SetCameraUniforms(Camera* camera);
            void SetFarNearUniforms(float near, float far);
            void SetModelUniforms(const glm::mat4& modelMatrix);
            void SetLightUniforms(const glm::vec3 pos, const glm::vec3 diffuse, const glm::mat4& lightViewProj);
            void SetLightUniforms(int lightIdx, const glm::vec3 pos, const glm::vec3 diffuse, const glm::mat4& lightViewProj);
            void SetCubemapGeomUniforms(const std::vector<glm::mat4>& lightViewProjMatrices);
            void SetLightAttenuationUniforms(const float constantAtt, const float linearAtt, const float quadraticAtt);
            void SetMaterialUniforms(const glm::vec3 diffuse, const glm::vec3 ambient, const glm::vec3 specular, const float shininess, const float reflectivity);

            void SetViewMatrix(const glm::mat4& viewMatrix);

            void SetDiffTexture(const int textureUnit);
            void SetCubeTexture(const int textureUnit);
            void SetDepthTexture(const int textureUnit);

            void SetSpecularTexture(const int textureUnit);
            void EnableSpecularTexture(const int useSpecular);
            void EnableDiffuseTexture(const int useDiffuse);
            void EnableCubemapTexture(const int useCubemap);

            void bind();
            void unbind();

            //generic setFunctions
            void SetBool(const std::string& name, bool value);
            void SetInt(const std::string& name, int value);
            void SetFloat(const std::string& name, float value);
            void SetVec2(const std::string& name, const glm::vec2& value);
            void SetVec2(const std::string& name, float x, float y);
            void SetVec3(const std::string& name, const glm::vec3& value);
            void SetVec3(const std::string& name, float x, float y, float z);
            void SetVec4(const std::string& name, const glm::vec4& value);
            void SetVec4(const std::string& name, float x, float y, float z, float w);
            void SetMat2(const std::string& name, const glm::mat2& mat);
            void SetMat3(const std::string& name, const glm::mat3& mat);
            void SetMat4(const std::string& name, const glm::mat4& mat);

        private:

            std::string includeIndentifier = "#include ";
            std::list<std::string> fileNames;
            std::string LoadShaderFile(const std::string& strPath, const std::string& strFilename, GLuint iShaderHandle, std::list<ShaderDefine> defines);

            void getMatricesUniformsLocation();
            void getCameraUniformsLocation();
            void getLightUniformsLocation();
            void getTexturesUniforms();
            void getMaterialUniformsLocation();


            // utility function for checking shader compilation/linking errors.
            void checkLinkingErrors();

            bool isBind = false;

            //UNIFORMS LOCATIONS
            //default proj + view + model matrices 
            int projMatrixLocation = -1;
            int viewMatrixLocation = -1;
            int modelMatrixLocation = -1;

            //camera uniform location
            int viewPosLocation = -1;
            int targetPosLocation = -1;
            int nearPlaneLocation = -1;
            int farPlaneLocation = -1;
            int fovLocation = -1;

            //standard light uniforms
            int uniformLightPositionLocation = -1;
            int uniformLightDiffuseLocation = -1;
            int uniformLightViewProjLocation = -1;  //view projection for directional shadow maps depth rendering
            int uniformLightNearLocation = -1;
            int uniformLightFarLocation = -1;

            int uniformLightConstantLocation = -1;
            int uniformLightLinearLocation = -1;
            int uniformLightQuadraticLocation = -1;

            //standard material uniforms
            int uniformMaterialAmbientLocation = -1;
            int uniformMaterialDiffuseLocation = -1;
            int uniformMaterialSpecularLocation = -1;
            int uniformMaterialShininessLocation = -1;
            int uniformMaterialReflectivityLocation = -1;

            //textures location
            int textureUseDiffuseLocation = -1;
            int textureDiffuseLocation = -1;
            int textureUseSpecularLocation = -1;
            int textureSpecularLocation = -1;
            int textureUseCubemapLocation = -1;
            int textureCubeLocation = -1;
            int textureDepthLocation = -1;

    };

}
#endif