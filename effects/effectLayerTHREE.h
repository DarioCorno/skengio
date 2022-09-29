#pragma once

#include "../skengio/core.h"
#include "../skengio/utils/geometries/sphere.h"

class EffectThree : public SKEngio::Layer {
    public:

    unsigned int vertexArray, vertexBuffer, indexBuffer;
    float fxparam = 1.0f;
    SKEngio::ShaderProgram* shaderProgram;
    SKEngio::TextureLoader* texture;

    SKEngio::Sphere* sphere;

    SKEngio::Light* light;

    //light uniforms location
    int uniformPositionLocation;
    int uniformAmbientLocation;
    int uniformDiffuseLocation;
    int uniformSpecularLocation;

    int projMatrixLocation = 0;
    int viewMatrixLocation = 0;
    int modelMatrixLocation = 0;    

    void OnAttach() {
        std::cout << "Layer " << this->GetId() << " attached. BEGIN creating buffers " << std::endl;

        sphere = new SKEngio::Sphere(1.0f, 12, 8, true);
        GLuint vboID;
        GLuint iboID;
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sphere->getInterleavedVertexSize(), sphere->getInterleavedVertices(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glGenBuffers(1, &iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->getIndexSize(), sphere->getIndices(), GL_STATIC_DRAW);        

        light = new SKEngio::Light();

        std::cout << "BEGIN loading shaders." << std::endl;

        shaderProgram = new SKEngio::ShaderProgram();
        shaderProgram->LoadShader("./effects/vShaderTHREE.txt", SKEngio::ShaderProgram::VERTEX);
        shaderProgram->LoadShader("./effects/fShaderTHREE.txt", SKEngio::ShaderProgram::FRAGMENT);
        shaderProgram->CreateProgram();

        std::cout << "END loading shaders." << std::endl;

        texture = new SKEngio::TextureLoader();
        texture->Load("./resources/textures/amiga.jpg", false);

        uniformPositionLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_POSITION_UNIFORM_NAME);
        uniformAmbientLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_AMBIENT_UNIFORM_NAME);
        uniformDiffuseLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_DIFFUSE_UNIFORM_NAME);
        uniformSpecularLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_SPECULAR_UNIFORM_NAME);  

        projMatrixLocation = glGetUniformLocation(shaderProgram->programID, PROJ_MATRIX_UNIFORM_NAME );
        viewMatrixLocation = glGetUniformLocation(shaderProgram->programID, VIEW_MATRIX_UNIFORM_NAME );
        modelMatrixLocation = glGetUniformLocation(shaderProgram->programID, MODEL_MATRIX_UNIFORM_NAME );            

    }

    void OnDetach() {
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader); 

        delete shaderProgram;
        delete texture;
        delete sphere;

        glDeleteVertexArrays(1, &vertexArray);

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {
               
    }

    void OnUpdate(float t) {
        glProgramUniform4fv(shaderProgram->programID, uniformPositionLocation, 1, glm::value_ptr( light->lightPosition ) );
        glProgramUniform4fv(shaderProgram->programID, uniformAmbientLocation, 1, glm::value_ptr( light->lightAmbientColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformDiffuseLocation, 1, glm::value_ptr( light->lightDiffuseColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformSpecularLocation, 1, glm::value_ptr( light->lightSpecularColor ) );


        //model matrix, identity
        glm::mat4 model = glm::mat4(1.0f); 
        model = glm::rotate( model, (t * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //move camera position and target, camera was set from renderer activeCamera
        activeCamera->setPosition( sin(t), 0.0f, -4.0f );
        activeCamera->setTarget( sin(t), 0.0f, 0.0f );

        //set the shader uniform matrices
        glProgramUniformMatrix4fv( shaderProgram->programID, projMatrixLocation, 1, GL_FALSE, glm::value_ptr( activeCamera->getProjMatrix() ) );
        glm::mat4 viewMat = activeCamera->getViewMatrix();
        glProgramUniformMatrix4fv( shaderProgram->programID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat) );
        glProgramUniformMatrix4fv( shaderProgram->programID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model) );
        sphere->draw();
    }

    void OnDraw(float t) {

        shaderProgram->Use();

        texture->bind();
        shaderProgram->Use();



        //glBindVertexArray(vertexArray);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    
};