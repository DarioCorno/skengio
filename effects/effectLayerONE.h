#pragma once

#include "../skengio/core.h"

class EffectOne : public SKEngio::Layer {
    public:

    unsigned int vertexArray, vertexBuffer, indexBuffer;
    float fxparam = 1.0f;
    SKEngio::ShaderProgram* shaderProgram;
    bool useLoader = true;
    int vertexColorLocation = 0;

    int projMatrixLocation = 0;
    int viewMatrixLocation = 0;
    int modelMatrixLocation = 0;


    void OnAttach() {
        std::cout << "Layer " << this->GetId() << " attached. BEGIN creating buffers " << std::endl;

        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        float vertices[] = {
            // positions        
            0.5f,  -0.5f, 0.0f,  
            -0.5f, -0.5f, 0.0f,  
            0.0f, 0.5f, 0.0f, 
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::cout << "BEGIN loading shaders." << std::endl;

        shaderProgram = new SKEngio::ShaderProgram();
        shaderProgram->LoadShader("./effects/", "vShaderONE.txt", SKEngio::ShaderProgram::VERTEX);
        shaderProgram->LoadShader("./effects/", "fShaderONE.txt", SKEngio::ShaderProgram::FRAGMENT);
        shaderProgram->CreateProgram();

        std::cout << "END loading shaders." << std::endl;

        //retrieve the address of the shader uniforms
        vertexColorLocation = glGetUniformLocation(shaderProgram->programID , "vertexColor");

        projMatrixLocation = glGetUniformLocation(shaderProgram->programID, PROJ_MATRIX_UNIFORM_NAME );
        viewMatrixLocation = glGetUniformLocation(shaderProgram->programID, VIEW_MATRIX_UNIFORM_NAME );
        modelMatrixLocation = glGetUniformLocation(shaderProgram->programID, MODEL_MATRIX_UNIFORM_NAME );

    }

    void OnDetach() {
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader); 

        delete shaderProgram;

        glDeleteVertexArrays(1, &vertexArray);

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {

        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver );
        ImGui::SetNextWindowBgAlpha(0.5f);

        ImGui::Begin("Effect ONE");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Flash speed:");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &fxparam, 0.1f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        ImGui::Text("ShaderProg ID: %i", shaderProgram->programID);

        ImGui::Text("Camera     ID: 1");

        ImGui::End();                    
    }

    void OnUpdate(float t) {

        //glUseProgram(shaderProgram);        

        float greenValue = sin(t * fxparam) / 2.0f + 0.5f;

        //OpenGL 4.1 specs
        //shaderProgram->Use();

        //set the value of shader variable ourColor
        //OpenGL4.1 specs (needs to activate program)
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //OpenGL 4.5 Specs (doesn't need to activate program, just pass the program id)
        glProgramUniform4f( shaderProgram->programID, vertexColorLocation, 0.0f, greenValue, 0.0f, 0.0f );

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
    }

    void OnDraw(float t) {

        glDisable(GL_BLEND);
        
        shaderProgram->bind();

        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    
};