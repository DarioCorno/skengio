#ifndef SHADER_H
#define SHADER_H

#include <skengio/core.h>

#include "../logger.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <skengio/material.h>

namespace SKEngio {

    class Material;

    //a simple class to load and compile shaders
    class ShaderProgram
    {
        public:
        
            enum SHADERTYPE {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION};

            unsigned int shaders[4];    //shaders IDs for every shader type
            unsigned int programID = 0;

            ShaderProgram();

            ~ShaderProgram();

            void LoadShader(std::string strPath, std::string strFileName, SHADERTYPE typeShader);

            void CreateProgram();

            void SetCameraUniforms(Camera* camera);
            void SetModelUniforms(glm::mat4 modelMatrix);
            void SetLightUniforms(Light* light);
            void SetMaterialUniforms(Material* mat);

            void SetViewMatrix(glm::mat4 viewMatrix);

            void SetDiffTexture(int textureID);
            void SetCubeTexture(int textureID);

            void bind();

            void unbind();

        private:

            std::string includeIndentifier = "#include ";

            std::string LoadShaderFile(std::string strPath, std::string strFilename, GLuint iShaderHandle);      

            void getMatricesUniformsLocation();
            void getLightUniformsLocation();
            void getTexturesUniform();
            void getMaterialUniformsLocation();


            // utility function for checking shader compilation/linking errors.
            void checkLinkingErrors();

            bool isBind;

            //UNIFORMS LOCATIONS
            //default proj + view + model matrices 
            int projMatrixLocation = -1;
            int viewMatrixLocation = -1;
            int modelMatrixLocation = -1;

            //viewpos uniform location
            int viewPosLocation = -1;

            //standard light uniforms
            int uniformLightPositionLocation = -1;
            int uniformLightDiffuseLocation = -1;

            //standard material uniforms
            int uniformMaterialAmbientLocation = -1;
            int uniformMaterialDiffuseLocation = -1;
            int uniformMaterialSpecularLocation = -1;
            int uniformMaterialShininessLocation = -1;
            int uniformMaterialReflectivityLocation = -1;
            int uniformMaterialTexOpacityLocation = -1;

            //textures location
            int textureDiffuseLocation = -1;
            int textureCubeLocation = -1;

    };

}
#endif