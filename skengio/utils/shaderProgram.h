#ifndef SHADER_H
#define SHADER_H

#include "../core.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>

namespace SKEngio {

    //a simple class to load and compile shaders
    class ShaderProgram
    {
    public:
        
        enum SHADERTYPE {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION};

        unsigned int shaders[4];
        unsigned int programID = 0;

        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        ShaderProgram()
        {
            shaders[VERTEX] = 0;
            shaders[FRAGMENT] = 0;
            shaders[GEOMETRY] = 0;
            shaders[TESSELATION] = 0;            
        }

        ~ShaderProgram() {
            std::cout << "Unloading Shader Program (ID: " << programID << ")" <<  std::endl;
            glDeleteProgram(programID);                   
        }

        void LoadShader(std::string strFileName, SHADERTYPE typeShader) {

            GLuint hShader = 0;
            GLint status;

            //Create shader object
            switch (typeShader)
            {
                case VERTEX : { hShader = glCreateShader(GL_VERTEX_SHADER); break; }
                case FRAGMENT : { hShader = glCreateShader(GL_FRAGMENT_SHADER); break; }
                case GEOMETRY : { hShader = glCreateShader(GL_GEOMETRY_SHADER); break; }
                case TESSELATION : { hShader = 0; std::cerr<<" TESSELLATION Shaders not implemented yet." << std::endl; }
            }

            if(LoadShaderFile(strFileName, hShader))
            {
                //now compile the shader
                glCompileShader(hShader);
                glGetShaderiv(hShader, GL_COMPILE_STATUS, &status);
                if(status == GL_FALSE)
                {
                    char infoLog[1024];
                    glGetShaderInfoLog(hShader, 1024, NULL, infoLog);
                    std::cout << "The shader at " << strFileName.c_str() << " failed to compile with the following errors:" << std::endl 
                    << infoLog << endl;
                    glDeleteShader(hShader);
                }
                else	//here, everything is OK
                {
                    std::cout << "The shader at " << strFileName.c_str() << " was compiled without errors." << std::endl;
                    shaders[typeShader] = hShader;
                }
            }
            else
            {
                std::cerr<< "ERROR! Something wrong loading the shader located in " << strFileName.c_str() << "." << std::endl;
                glDeleteShader(hShader);
            }
        }

        bool LoadShaderFile(std::string strFilename, GLuint iShaderHandle) {

            std::ifstream shaderSource(strFilename.c_str());
            if (!shaderSource.is_open())
            {
                std::cerr<< " File not found " << strFilename.c_str()<< endl;
                char tmp[256];
                getcwd(tmp, 256);
                std::cout << "Current working directory: " << tmp << std::endl;                
                return false;
            }
            // now read in the data
            string strSource = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
            shaderSource.close();
            strSource+="\0";
            //pass the code to OGL
            const char* data=strSource.c_str();
            glShaderSource(iShaderHandle, 1, &data, NULL);

            return true;

        }        

        void CreateProgram() {
            // shader Program
            programID = glCreateProgram();
            //attach the shaders
            if(shaders[VERTEX] > 0)
                glAttachShader(programID, shaders[VERTEX]);
            if(shaders[FRAGMENT] > 0)
                glAttachShader(programID, shaders[FRAGMENT]);
            if(shaders[GEOMETRY] > 0)
                glAttachShader(programID, shaders[GEOMETRY]);

            glLinkProgram(programID);
            checkLinkingErrors();
            
            //delete the shaders
            glDeleteShader(shaders[VERTEX]);
            glDeleteShader(shaders[FRAGMENT]);
            glDeleteShader(shaders[GEOMETRY]);

        }

        //should I remove that stuff?
        // activate the shader
        // ------------------------------------------------------------------------
        void Use() 
        { 
            glUseProgram(programID); 
        }

        void Disable() {
            glUseProgram(0);
        }

    private:

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkLinkingErrors()
        {

            GLint infologLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infologLength);
            //std::cerr<<"Link Log Length "<<infologLength<<"\n";

            if(infologLength > 1)
            {
                char *infoLog = new char[infologLength];
                GLint charsWritten  = 0;

                glGetProgramInfoLog(programID, infologLength, &charsWritten, infoLog);

                std::cerr << infoLog << std::endl;
                delete [] infoLog;
                glGetProgramiv(programID, GL_LINK_STATUS, &infologLength);
                if(infologLength == GL_FALSE)
                {
                    std::cerr << "Program link failed, exiting." << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cout << "Shader program linked correctly " << programID << std::endl;
            }
        }
    };

}
#endif