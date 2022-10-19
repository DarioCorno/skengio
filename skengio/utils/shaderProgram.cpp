#include "shaderProgram.h"
#include <direct.h>

#include "skengio/defines.h"

namespace SKEngio {

    ShaderProgram::ShaderProgram() {
        shaders[VERTEX] = 0;
        shaders[FRAGMENT] = 0;
        shaders[GEOMETRY] = 0;
        shaders[TESSELATION] = 0; 

        projMatrixLocation = -1;
        viewMatrixLocation = -1;
        modelMatrixLocation = -1;

        viewPosLocation = -1;

        uniformLightPositionLocation = -1;
        uniformLightDiffuseLocation = -1;

        uniformMaterialAmbientLocation = -1;
        uniformMaterialDiffuseLocation = -1;
        uniformMaterialSpecularLocation = -1;
        uniformMaterialShininessLocation = -1;
        uniformMaterialReflectivityLocation = -1;
        uniformMaterialTexOpacityLocation = -1;

        isBind = false;
    }

    ShaderProgram::~ShaderProgram() {
        SK_LOG("Unloading Shader Program (ID: " << programID << ")" );
        glDeleteProgram(programID);                   
    }

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
                SK_LOG_ERR(infoLog);
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
            SK_LOG_ERR("ERROR! Something wrong loading the shader located in " << strFileName.c_str() << ".");
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
            _getcwd(tmp, 256);
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
        
        bind();
        //retrieve all the available uniforms in current program
        getMatricesUniformsLocation();
        getLightUniformsLocation();
        getMaterialUniformsLocation();
        getTexturesUniform();

        //delete the shaders
        glDeleteShader(shaders[VERTEX]);
        glDeleteShader(shaders[FRAGMENT]);
        glDeleteShader(shaders[GEOMETRY]);
    }

    void ShaderProgram::SetCameraUniforms(Camera* camera) {
        if(projMatrixLocation != -1)
            glProgramUniformMatrix4fv(programID, projMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->getProjMatrix()));

        if (viewMatrixLocation != -1) {
            glm::mat4 viewMat = camera->getViewMatrix();
            glProgramUniformMatrix4fv(programID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
        }

        if(viewPosLocation != -1)
            glProgramUniform3fv(programID, viewPosLocation, 1, glm::value_ptr(camera->position));
    }

    void ShaderProgram::SetModelUniforms(glm::mat4 modelMatrix) {
        if(modelMatrixLocation != -1)
            glProgramUniformMatrix4fv(programID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }

    void ShaderProgram::SetLightUniforms(Light* light) {
        if(uniformLightPositionLocation != -1)
            glProgramUniform3fv(programID, uniformLightPositionLocation, 1, glm::value_ptr(light->GetPosition()));

        if(uniformLightDiffuseLocation != -1)
            glProgramUniform3fv(programID, uniformLightDiffuseLocation, 1, glm::value_ptr(light->GetDiffuse()));

    }

    void ShaderProgram::SetMaterialUniforms(Material* material) {
        if (uniformMaterialDiffuseLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialDiffuseLocation, 1, glm::value_ptr(material->GetDiffuse()));

        if (uniformMaterialAmbientLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialAmbientLocation, 1, glm::value_ptr(material->GetAmbient()));

        if (uniformMaterialSpecularLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialSpecularLocation, 1, glm::value_ptr(material->GetSpecular()));

        if (uniformMaterialShininessLocation != -1)
            glProgramUniform1f(programID, uniformMaterialShininessLocation, material->GetShininess() );

        if (uniformMaterialReflectivityLocation != -1)
            glProgramUniform1f(programID, uniformMaterialReflectivityLocation, material->GetReflectivity());

    }

    void ShaderProgram::SetViewMatrix(glm::mat4 viewMatrix) {
        if (viewMatrixLocation != -1) {
            glProgramUniformMatrix4fv(programID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        }
    }

    void ShaderProgram::SetDiffTexture(int textureID) {
        if (textureDiffuseLocation != -1) {
            glProgramUniform1i(programID, textureDiffuseLocation, textureID);
        }
    }

    void ShaderProgram::SetCubeTexture(int textureID) {
        if (!isBind)
            bind();

        if (textureCubeLocation != -1) {
            glProgramUniform1i(programID, textureCubeLocation, textureID);
        }
    }

    void ShaderProgram::getTexturesUniform() {
        textureDiffuseLocation = glGetUniformLocation(programID, TEXTURE_DIFFUSE_UNIFORM_NAME);
        textureCubeLocation = glGetUniformLocation(programID, TEXTURE_CUBEMAP_UNIFORM_NAME);
    }


    void ShaderProgram::getMatricesUniformsLocation() {
        //this program must be in use
        projMatrixLocation = glGetUniformLocation(programID, PROJ_MATRIX_UNIFORM_NAME);
        viewMatrixLocation = glGetUniformLocation(programID, VIEW_MATRIX_UNIFORM_NAME);
        modelMatrixLocation = glGetUniformLocation(programID, MODEL_MATRIX_UNIFORM_NAME);

        viewPosLocation = glGetUniformLocation(programID, CAMERA_VIEWPOS_UNIFORM_NAME);
    }

    void ShaderProgram::getLightUniformsLocation() {
        //this program must be in use
        uniformLightPositionLocation = glGetUniformLocation(programID, LIGHT_POSITION_UNIFORM_NAME);
        uniformLightDiffuseLocation = glGetUniformLocation(programID, LIGHT_DIFFUSE_UNIFORM_NAME);
    }

    void ShaderProgram::getMaterialUniformsLocation() {
        uniformMaterialAmbientLocation = glGetUniformLocation(programID, MATERIAL_AMBIENT_UNIFORM_NAME);
        uniformMaterialDiffuseLocation = glGetUniformLocation(programID, MATERIAL_DIFFUSE_UNIFORM_NAME);
        uniformMaterialSpecularLocation = glGetUniformLocation(programID, MATERIAL_SPECULAR_UNIFORM_NAME);
        uniformMaterialShininessLocation = glGetUniformLocation(programID, MATERIAL_SHININESS_UNIFORM_NAME);
        uniformMaterialReflectivityLocation = glGetUniformLocation(programID, MATERIAL_REFLECTIVITY_UNIFORM_NAME);
    }

    void ShaderProgram::bind() 
    { 
        if (programID > 0) {
            glUseProgram(programID);
            isBind = true;
        }
        else {
            isBind = false;
        }
    }

    void ShaderProgram::unbind() {
        glUseProgram(0);
        isBind = false;
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
