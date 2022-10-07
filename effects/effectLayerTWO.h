#pragma once

#include "../skengio/core.h"

class EffectTwo : public SKEngio::Layer {
    public:

    unsigned int VBO, VAO;
    unsigned int vertexShader, fragmentShader;
    glm::mat4 model = glm::mat4(1.0f);  //model matrix
    int projMatrixLocation = 0;
    int viewMatrixLocation = 0;
    int modelMatrixLocation = 0;

    //light uniforms location
    int uniformPositionLocation = 0;
    int uniformAmbientLocation = 0;
    int uniformDiffuseLocation = 0;
    int uniformSpecularLocation = 0;

    SKEngio::Sphere* mesh;
    SKEngio::ShaderProgram* shaderProgram;
    SKEngio::TextureLoader* texture;
    SKEngio::Light* light;

    void OnAttach() {
        std::cout << "Layer attached, creating buffers " << this->GetId() << std::endl;

        shaderProgram = new SKEngio::ShaderProgram();
        shaderProgram->LoadShader("./effects/vShaderTWO.txt", SKEngio::ShaderProgram::VERTEX);
        shaderProgram->LoadShader("./effects/fShaderTWO.txt", SKEngio::ShaderProgram::FRAGMENT);
        shaderProgram->CreateProgram();

        texture = new SKEngio::TextureLoader();
        texture->Load("./resources/textures/earth.jpg", false);


        light = new SKEngio::Light();

        mesh = new SKEngio::Sphere();
        mesh->Generate(1.0f, 2);
        mesh->buildInterleavedArray();
        mesh->createGLBuffers();

        glUseProgram(shaderProgram->programID);
        projMatrixLocation = glGetUniformLocation(shaderProgram->programID, PROJ_MATRIX_UNIFORM_NAME );
        viewMatrixLocation = glGetUniformLocation(shaderProgram->programID, VIEW_MATRIX_UNIFORM_NAME );
        modelMatrixLocation = glGetUniformLocation(shaderProgram->programID, MODEL_MATRIX_UNIFORM_NAME );

        uniformPositionLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_POSITION_UNIFORM_NAME);
        uniformDiffuseLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_DIFFUSE_UNIFORM_NAME);
        uniformAmbientLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_AMBIENT_UNIFORM_NAME);
        //uniformSpecularLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_SPECULAR_UNIFORM_NAME);  


    }

    void OnDetach() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete shaderProgram;
        delete mesh;        
        delete texture;
        delete light;

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {

    }

    void OnUpdate(float t) {
        //OpenGL 4.1 specs (4.5 specs doesn't need to use the program before setting uniforms)
        //glUseProgram(shaderProgramID);        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)(t * 80.0f), glm::vec3(0.0f, 1.0f, 0.0f));        
        //glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));        

        
        //move camera position and target, camera was set from renderer activeCamera
        activeCamera->setPosition( 0.0f, 0.0f, -4.0f );
        activeCamera->setTarget( 0.0f, 0.0f, 0.0f );

        //set the shader uniform matrices
        glProgramUniformMatrix4fv( shaderProgram->programID, projMatrixLocation, 1, GL_FALSE, glm::value_ptr( activeCamera->getProjMatrix() ) );
        glm::mat4 viewMat = activeCamera->getViewMatrix();
        glProgramUniformMatrix4fv( shaderProgram->programID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat) );
        glProgramUniformMatrix4fv( shaderProgram->programID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model) );

        glProgramUniform3fv( shaderProgram->programID, uniformPositionLocation, 1, glm::value_ptr(light->GetPosition() ));
        glProgramUniform3fv( shaderProgram->programID, uniformDiffuseLocation, 1, glm::value_ptr(light->GetDiffuse() ));
        glProgramUniform3fv( shaderProgram->programID, uniformAmbientLocation, 1, glm::value_ptr(light->GetAmbient() ));

    }

    void OnDraw(float t) {
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_ONE, GL_ONE);

        texture->bind();

        glUseProgram(shaderProgram->programID);        

        mesh->draw();
    }

    
};