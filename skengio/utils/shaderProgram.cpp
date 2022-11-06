#include "shaderProgram.h"
#include <direct.h>
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "skengio/defines.h"

namespace SKEngio {

    ShaderProgram::~ShaderProgram() {
        SK_LOG("Unloading Shader Program (ID: " << programID << ")" );
        glDeleteProgram(programID);                   
    }

    void ShaderProgram::OnDestroy() {

    }
    void ShaderProgram::LoadShader(const std::string& strPath, const std::string& strFileName, SHADERTYPE typeShader) {
        std::list<ShaderDefine> defines;
        defines.clear();
        LoadShader(strPath, strFileName, typeShader, defines);
    }

    void ShaderProgram::LoadShader(const std::string& strPath, const std::string& strFileName, SHADERTYPE typeShader, std::list<ShaderDefine> defines) {

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
        std::string fullSource = LoadShaderFile(strPath, strFileName, handleShader, defines);        

        if(fullSource.length() > 0)
        {
            const char* data = fullSource.c_str();
            glShaderSource(handleShader, 1, &data , nullptr);

            //now compile the shader
            glCompileShader(handleShader);
            glGetShaderiv(handleShader, GL_COMPILE_STATUS, &status);
            if(status == GL_FALSE)
            {
                char infoLog[1024];
                glGetShaderInfoLog(handleShader, 1024, nullptr, infoLog);
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

    std::string ShaderProgram::LoadShaderFile(const std::string& strPath, const std::string& strFilename, GLuint iShaderHandle, std::list<ShaderDefine> defines) {
        
        std::string strPathFilename = strPath + strFilename;
        fileName = strPathFilename;
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
        std::string fullSourceCode;
        std::ifstream file(strPathFilename)        ;

		if (!file.is_open())
		{
			SK_LOG_ERR("ERROR: could not open the shader at: " << strPathFilename);

			return fullSourceCode;
		}        


		std::string lineBuffer;
        int foundDefines = 0;
        int setDefines = 0;
		while (std::getline(file, lineBuffer))
		{
			// Look for the new shader include identifier
			if (lineBuffer.find(includeIndentifier) != std::string::npos)
			{
				// Remove the include identifier, this will cause the path to remain
				lineBuffer.erase(0, includeIndentifier.size());

				fullSourceCode += LoadShaderFile(strPath, lineBuffer, iShaderHandle, defines);

				// Do not add this line to the shader source code, as the include
				// path would generate a compilation issue in the final source code
				continue;
			}

            //replace all define values
            foundDefines += ((lineBuffer.find("#willdefine") != std::string::npos) ? 1 : 0);
            for (ShaderDefine sdef : defines) {
                if (lineBuffer.find( "#willdefine " + sdef.defineName) != std::string::npos) {
                    lineBuffer = "#define " + sdef.defineName + " " + sdef.defineValue;
                    setDefines += 1;
                }
            }

			fullSourceCode += lineBuffer + '\n';
		}

        file.close();

        if (foundDefines != setDefines) {
            SK_LOG_ERR("WARNING! Defines mismatch in " << strPathFilename.c_str());
        }

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
        getCameraUniformsLocation();
        getLightUniformsLocation();
        getMaterialUniformsLocation();
        getTexturesUniforms();

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

        if (viewPosLocation != -1)
            glProgramUniform3fv(programID, viewPosLocation, 1, glm::value_ptr(camera->position));

        if(targetPosLocation != -1)
            glProgramUniform3fv(programID, targetPosLocation, 1, glm::value_ptr(camera->target));

        if (nearPlaneLocation != -1)
            glProgramUniform1f(programID, nearPlaneLocation, camera->nearPlane);

        if (farPlaneLocation != -1)
            glProgramUniform1f(programID, farPlaneLocation, camera->farPlane);

        if (fovLocation != -1)
            glProgramUniform1f(programID, fovLocation, camera->fieldOfView);

    }


    void ShaderProgram::SetFarNearUniforms(float near, float far) {
        if (nearPlaneLocation != -1)
            glProgramUniform1f(programID, nearPlaneLocation, near);

        if (farPlaneLocation != -1)
            glProgramUniform1f(programID, farPlaneLocation, far);
    }

    void ShaderProgram::SetModelUniforms(const glm::mat4& modelMatrix) {
        if(modelMatrixLocation != -1)
            glProgramUniformMatrix4fv(programID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }

    void ShaderProgram::SetLightUniforms(const glm::vec3 pos, const glm::vec3 diffuse, const glm::mat4& lightViewProj) {
        if(uniformLightPositionLocation != -1)
            glProgramUniform3fv(programID, uniformLightPositionLocation, 1, glm::value_ptr( pos ));

        if(uniformLightDiffuseLocation != -1)
            glProgramUniform3fv(programID, uniformLightDiffuseLocation, 1, glm::value_ptr( diffuse ));

        if (uniformLightViewProjLocation != -1)
            glProgramUniformMatrix4fv(programID, uniformLightViewProjLocation, 1, GL_FALSE, glm::value_ptr( lightViewProj ));
    }

    void ShaderProgram::SetMaterialUniforms(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float shininess, float reflectivity) {
        if (uniformMaterialDiffuseLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialDiffuseLocation, 1, glm::value_ptr( diffuse ));

        if (uniformMaterialAmbientLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialAmbientLocation, 1, glm::value_ptr( ambient ));

        if (uniformMaterialSpecularLocation != -1)
            glProgramUniform3fv(programID, uniformMaterialSpecularLocation, 1, glm::value_ptr( specular ));

        if (uniformMaterialShininessLocation != -1)
            glProgramUniform1f(programID, uniformMaterialShininessLocation, shininess );

        if (uniformMaterialReflectivityLocation != -1)
            glProgramUniform1f(programID, uniformMaterialReflectivityLocation, reflectivity );

    }

    void ShaderProgram::SetViewMatrix(const glm::mat4& viewMatrix) {
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

    void ShaderProgram::SetDepthTexture(int textureID) {
        if (!isBind)
            bind();

        if (textureDepthLocation != -1) {
            glProgramUniform1i(programID, textureDepthLocation, textureID);
        }
    }

    void ShaderProgram::getTexturesUniforms() {
        textureDiffuseLocation = glGetUniformLocation(programID, TEXTURE_DIFFUSE_UNIFORM_NAME);
        textureCubeLocation = glGetUniformLocation(programID, TEXTURE_CUBEMAP_UNIFORM_NAME);
        textureDepthLocation = glGetUniformLocation(programID, TEXTURE_DEPTH_UNIFORM_NAME);
    }


    void ShaderProgram::getMatricesUniformsLocation() {
        //this program must be in use
        projMatrixLocation = glGetUniformLocation(programID, PROJ_MATRIX_UNIFORM_NAME);
        viewMatrixLocation = glGetUniformLocation(programID, VIEW_MATRIX_UNIFORM_NAME);
        modelMatrixLocation = glGetUniformLocation(programID, MODEL_MATRIX_UNIFORM_NAME);
    }

    void ShaderProgram::getCameraUniformsLocation() {
        //this program must be in use
        viewPosLocation = glGetUniformLocation(programID, CAMERA_VIEWPOS_UNIFORM_NAME);
        targetPosLocation = glGetUniformLocation(programID, CAMERA_TARGETPOS_UNIFORM_NAME);

        nearPlaneLocation = glGetUniformLocation(programID, CAMERA_NEAR_PLANE_UNIFORM_NAME);
        farPlaneLocation = glGetUniformLocation(programID, CAMERA_FAR_PLANE_UNIFORM_NAME);
        fovLocation = glGetUniformLocation(programID, CAMERA_FOV_UNIFORM_NAME);
    }

    void ShaderProgram::getLightUniformsLocation() {
        //this program must be in use
        uniformLightPositionLocation = glGetUniformLocation(programID, LIGHT_POSITION_UNIFORM_NAME);
        uniformLightDiffuseLocation = glGetUniformLocation(programID, LIGHT_DIFFUSE_UNIFORM_NAME);
        uniformLightViewProjLocation = glGetUniformLocation(programID, LIGHT_VIEWPROJMATRIX_UNIFORM_NAME);
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

    void ShaderProgram::SetBool(const std::string& name, bool value) 
    {
        glProgramUniform1i(programID, glGetUniformLocation(programID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetInt(const std::string& name, int value) 
    {
        glProgramUniform1i(programID, glGetUniformLocation(programID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetFloat(const std::string& name, float value) 
    {
        glProgramUniform1f(programID, glGetUniformLocation(programID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetVec2(const std::string& name, const glm::vec2& value) 
    {
        glProgramUniform2fv(programID, glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    }
    void ShaderProgram::SetVec2(const std::string& name, float x, float y) 
    {
        glProgramUniform2f(programID, glGetUniformLocation(programID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& value) 
    {
        glProgramUniform3fv(programID, glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    }
    void ShaderProgram::SetVec3(const std::string& name, float x, float y, float z) 
    {
        glProgramUniform3f(programID, glGetUniformLocation(programID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetVec4(const std::string& name, const glm::vec4& value) 
    {
        glProgramUniform4fv(programID, glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    }
    void ShaderProgram::SetVec4(const std::string& name, float x, float y, float z, float w) 
    {
        glProgramUniform4f(programID, glGetUniformLocation(programID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetMat2(const std::string& name, const glm::mat2& mat) 
    {
        glProgramUniformMatrix2fv(programID, glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetMat3(const std::string& name, const glm::mat3& mat) 
    {
        glProgramUniformMatrix3fv(programID, glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& mat) 
    {
        glProgramUniformMatrix4fv(programID, glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

}
