#ifndef SHADER_H
#define SHADER_H

#include "../logger.h"

#include <string>

#include <skengio/material.h>

#define GLEW_STATIC
#include "GLEW/glew.h"
#include "skengio/camera.h"

namespace SKEngio {
    
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

            void CreateProgram();

            void SetCameraUniforms(Camera* camera);
            void SetModelUniforms(const glm::mat4& modelMatrix);
            void SetLightUniforms(const glm::vec3 pos, const glm::vec3 diffuse, const glm::mat4& lightViewProj);
            void SetMaterialUniforms(Material* mat);

            void SetViewMatrix(const glm::mat4& viewMatrix);

            void SetDiffTexture(int textureID);
            void SetCubeTexture(int textureID);
            void SetDepthTexture(int textureID);

            void bind();

            void unbind();

        private:

            std::string includeIndentifier = "#include ";
            std::string fileName = "";
            std::string LoadShaderFile(const std::string& strPath, const std::string& strFilename, GLuint iShaderHandle);      

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
            int uniformLightViewProjLocation = -1;  //view projection for shadow maps depth rendering

            //standard material uniforms
            int uniformMaterialAmbientLocation = -1;
            int uniformMaterialDiffuseLocation = -1;
            int uniformMaterialSpecularLocation = -1;
            int uniformMaterialShininessLocation = -1;
            int uniformMaterialReflectivityLocation = -1;

            //textures location
            int textureDiffuseLocation = -1;
            int textureCubeLocation = -1;
            int textureDepthLocation = -1;

    };

}
#endif