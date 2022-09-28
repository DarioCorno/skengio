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
            SK_LOG("Unloading Shader Program (ID: " << programID << ")" );
            glDeleteProgram(programID);                   
        }

        void LoadShader(std::string strFileName, SHADERTYPE typeShader) {

            GLuint handleShader = 0;
            GLint status;

            //Create shader object
            switch (typeShader)
            {
                case VERTEX : { handleShader = glCreateShader(GL_VERTEX_SHADER); break; }
                case FRAGMENT : { handleShader = glCreateShader(GL_FRAGMENT_SHADER); break; }
                case GEOMETRY : { handleShader = glCreateShader(GL_GEOMETRY_SHADER); break; }
                case TESSELATION : { handleShader = 0; SK_LOG("WARNING!" << " TESSELLATION Shaders not implemented yet."); }
            }

            if(LoadShaderFile(strFileName, handleShader))
            {
                //now compile the shader
                glCompileShader(handleShader);
                glGetShaderiv(handleShader, GL_COMPILE_STATUS, &status);
                if(status == GL_FALSE)
                {
                    char infoLog[1024];
                    glGetShaderInfoLog(handleShader, 1024, NULL, infoLog);
                    SK_LOG("ERROR! The shader at " << strFileName.c_str() << " failed to compile with the following errors:"); 
                    SK_LOG(infoLog);
                    glDeleteShader(handleShader);
                }
                else	//here, everything is OK
                {
                    SK_LOG("The shader at " << strFileName.c_str() << " was compiled without errors.");
                    shaders[typeShader] = handleShader;
                }
            }
            else
            {
                SK_LOG("ERROR! Something wrong loading the shader located in " << strFileName.c_str() << ".");
                glDeleteShader(handleShader);
            }
        }

        bool LoadShaderFile(std::string strFilename, GLuint iShaderHandle) {

            std::ifstream shaderSource(strFilename.c_str());
            if (!shaderSource.is_open())
            {
                SK_LOG( "ERROR! File not found " << strFilename.c_str() );
                char tmp[256];
                getcwd(tmp, 256);
                SK_LOG("Current working directory: " << tmp);                
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

            if(infologLength > 1)
            {
                char *infoLog = new char[infologLength];
                GLint charsWritten  = 0;

                glGetProgramInfoLog(programID, infologLength, &charsWritten, infoLog);

                std::cout << infoLog << std::endl;

                delete [] infoLog;
                glGetProgramiv(programID, GL_LINK_STATUS, &infologLength);
                if(infologLength == GL_FALSE)
                {
                    SK_LOG("ERROR! Program " << programID << "link failed, exiting.");
                    exit(EXIT_FAILURE);
                }
            } else {
                SK_LOG("Shader program linked correctly " << programID);
            }
        }
    };

}
#endif