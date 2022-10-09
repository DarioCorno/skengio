#ifndef SHADER_H
#define SHADER_H

#include "../core.h"

#include "../logger.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <fstream>

namespace SKEngio {

    //a simple class to load and compile shaders
    class ShaderProgram
    {
    public:
        
        enum SHADERTYPE {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION};

        unsigned int shaders[4];    //shaders ID for every shader type
        unsigned int programID = 0;

        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        ShaderProgram();

        ~ShaderProgram();

        void LoadShader(std::string strPath, std::string strFileName, SHADERTYPE typeShader);

        void CreateProgram();

        void bind();

        void unbind();

    private:

        std::string includeIndentifier = "#include ";

        std::string LoadShaderFile(std::string strPath, std::string strFilename, GLuint iShaderHandle);      

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkLinkingErrors();

    };

}
#endif