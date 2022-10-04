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

    SKEngio::Material* material;

    //light uniforms location
    int uniformPositionLocation;
    int uniformAmbientLocation;
    int uniformDiffuseLocation;
    int uniformSpecularLocation;

    int projMatrixLocation = 0;
    int viewMatrixLocation = 0;
    int modelMatrixLocation = 0;    

    int uniformMaterialAmbientLocation = 0;
    int uniformMaterialDiffuseLocation = 0;
    int uniformMaterialSpecularLocation = 0;
    int uniformMaterialShininessLocation = 0;

    void OnAttach() {
        std::cout << "Layer " << this->GetId() << " attached. BEGIN creating buffers " << std::endl;

        sphere = new SKEngio::Sphere(1.0f, 12, 8, true);
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glBufferData(GL_ARRAY_BUFFER, sphere->getInterleavedVertexSize(), sphere->getInterleavedVertices(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(attribVertexPosition);
        glEnableVertexAttribArray(attribVertexNormal);
        glEnableVertexAttribArray(attribVertexTexCoord);

        int stride = sphere1.getInterleavedStride();
        glVertexAttribPointer(attribVertexPosition, 3, GL_FLOAT, false, stride, 0);
        glVertexAttribPointer(attribVertexNormal, 3, GL_FLOAT, false, stride, (void*)(3 * sizeof(float)));
        glVertexAttribPointer(attribVertexTexCoord, 2, GL_FLOAT, false, stride, (void*)(6 * sizeof(float)));

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        texture = new SKEngio::TextureLoader();
        texture->Load("./resources/textures/amiga.jpg", false);

        /*
        light = new SKEngio::Light();

        std::cout << "BEGIN loading shaders." << std::endl;

        shaderProgram = new SKEngio::ShaderProgram();
        shaderProgram->LoadShader("./effects/vShaderTHREE.txt", SKEngio::ShaderProgram::VERTEX);
        shaderProgram->LoadShader("./effects/fShaderTHREE.txt", SKEngio::ShaderProgram::FRAGMENT);
        shaderProgram->CreateProgram();

        std::cout << "END loading shaders." << std::endl;

        uniformPositionLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_POSITION_UNIFORM_NAME);
        uniformAmbientLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_AMBIENT_UNIFORM_NAME);
        uniformDiffuseLocation   = glGetUniformLocation(shaderProgram->programID , LIGHT_DIFFUSE_UNIFORM_NAME);
        uniformSpecularLocation  = glGetUniformLocation(shaderProgram->programID , LIGHT_SPECULAR_UNIFORM_NAME);  

        projMatrixLocation = glGetUniformLocation(shaderProgram->programID, PROJ_MATRIX_UNIFORM_NAME );
        viewMatrixLocation = glGetUniformLocation(shaderProgram->programID, VIEW_MATRIX_UNIFORM_NAME );
        modelMatrixLocation = glGetUniformLocation(shaderProgram->programID, MODEL_MATRIX_UNIFORM_NAME );  
        
        material = new SKEngio::Material();

        uniformMaterialAmbientLocation    = glGetUniformLocation(shaderProgram->programID, MATERIAL_AMBIENT_UNIFORM_NAME);
        uniformMaterialDiffuseLocation    = glGetUniformLocation(shaderProgram->programID, MATERIAL_DIFFUSE_UNIFORM_NAME);
        uniformMaterialSpecularLocation   = glGetUniformLocation(shaderProgram->programID, MATERIAL_SPECULAR_UNIFORM_NAME);
        uniformMaterialShininessLocation  = glGetUniformLocation(shaderProgram->programID, MATERIAL_SHININESS_UNIFORM_NAME);                  
        */
    }

    void OnDetach() {
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader); 

        //delete shaderProgram;
        delete texture;
        delete sphere;

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {
               
    }

    void OnUpdate(float t) {
        /*
        //update light uniforms
        glProgramUniform4fv(shaderProgram->programID, uniformPositionLocation, 1, glm::value_ptr( light->lightPosition ) );
        glProgramUniform4fv(shaderProgram->programID, uniformAmbientLocation, 1, glm::value_ptr( light->lightAmbientColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformDiffuseLocation, 1, glm::value_ptr( light->lightDiffuseColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformSpecularLocation, 1, glm::value_ptr( light->lightSpecularColor ) );

        //update material uniforms
        glProgramUniform4fv(shaderProgram->programID, uniformMaterialAmbientLocation, 1, glm::value_ptr( material->materialAmbientColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformMaterialDiffuseLocation, 1, glm::value_ptr( material->materialDiffuseColor ) );
        glProgramUniform4fv(shaderProgram->programID, uniformMaterialSpecularLocation, 1, glm::value_ptr( material->materialSpecularColor ) );
        glProgramUniform1f(shaderProgram->programID, uniformMaterialShininessLocation, material->materialShininess );

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
        */


    }

    void OnDraw(float t) {

        //shaderProgram->Use();

        texture->bind();
        //shaderProgram->Use();

        // bind vbo for flat sphere (left)
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

        glMatrixMode(GL_MODELVIEW);
        glTranslatef(0.0f, 0.0f, 5.0f);

        // draw left sphere
        glDrawElements(GL_TRIANGLES,            // primitive type
                    sphere->getIndexCount(), // # of indices
                    GL_UNSIGNED_INT,         // data type
                    (void*)0);               // ptr to indices

        //glBindVertexArray(vertexArray);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    
};