#include "shaderProgram.h"

namespace SKEngio {

    ShaderProgram::ShaderProgram() {
        shaders[VERTEX] = 0;
        shaders[FRAGMENT] = 0;
        shaders[GEOMETRY] = 0;
        shaders[TESSELATION] = 0; 
    };

    ShaderProgram::~ShaderProgram() {
        SK_LOG("Unloading Shader Program (ID: " << programID << ")" );
        glDeleteProgram(programID);                   
    };

    void ShaderProgram::LoadShader(std::string strPath, std::string strFileName, SHADERTYPE typeShader) {

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

        //load the shader file
        std::string fullSource = LoadShaderFile(strPath, strFileName, handleShader);
        if(fullSource.length() > 0)
        {
            const char *data = fullSource.c_str();
            glShaderSource(handleShader, 1, &data , NULL);

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

    std::string ShaderProgram::LoadShaderFile(std::string strPath, std::string strFilename, GLuint iShaderHandle) {
        
        std::string strPathFilename = strPath + strFilename;
        std::ifstream shaderSource(strPathFilename.c_str());
        if (!shaderSource.is_open())
        {
            SK_LOG_ERR( "ERROR! File not found " << strPathFilename.c_str() );
            char tmp[256];
            getcwd(tmp, 256);
            SK_LOG_ERR("Current working directory: " << tmp);                
            return "";
        }
        // now read in the data
        std::string fullSourceCode = "";
        std::ifstream file(strPathFilename)        ;

		if (!file.is_open())
		{
			SK_LOG_ERR("ERROR: could not open the shader at: " << strPathFilename);

			return fullSourceCode;
		}        


		std::string lineBuffer;
		while (std::getline(file, lineBuffer))
		{
			// Look for the new shader include identifier
			if (lineBuffer.find(includeIndentifier) != lineBuffer.npos)
			{
				// Remove the include identifier, this will cause the path to remain
				lineBuffer.erase(0, includeIndentifier.size());

				fullSourceCode += LoadShaderFile(strPath, lineBuffer, iShaderHandle);

				// Do not add this line to the shader source code, as the include
				// path would generate a compilation issue in the final source code
				continue;
			}

			fullSourceCode += lineBuffer + '\n';
		}

        file.close();

        /* OLD CODE
        string strSource = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());        
        shaderSource.close();
        strSource+="\0";
        //pass the code to OGL
        const char* data=strSource.c_str();
        */
        

        return fullSourceCode;

    }        

    void ShaderProgram::CreateProgram() {
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

    void ShaderProgram::bind() 
    { 
        if(programID > 0)
            glUseProgram(programID); 
    }

    void ShaderProgram::unbind() {
        glUseProgram(0);
    }

    void ShaderProgram::checkLinkingErrors()
    {

        GLint infologLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infologLength);

        if(infologLength > 1)
        {
            char *infoLog = new char[infologLength];
            GLint charsWritten  = 0;

            glGetProgramInfoLog(programID, infologLength, &charsWritten, infoLog);

            SK_LOG_ERR(infoLog);

            delete [] infoLog;
            glGetProgramiv(programID, GL_LINK_STATUS, &infologLength);
            if(infologLength == GL_FALSE)
            {
                SK_LOG_ERR("ERROR! Program " << programID << "link failed, exiting.");
                exit(EXIT_FAILURE);
            }
        } else {
            SK_LOG("Shader program linked correctly " << programID);
        }
    }


}