#pragma once

#include "../skengio/core.h"

class EffectThree : public SKEngio::Layer {
    public:

    unsigned int vertexArray, vertexBuffer, indexBuffer;
    float fxparam = 1.0f;
    SKEngio::ShaderProgram* shaderProgram;
    SKEngio::TextureLoader* texture;

    void OnAttach() {
        std::cout << "Layer " << this->GetId() << " attached. BEGIN creating buffers " << std::endl;

        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        float vertices[] = {
            // positions            // colors                 // texture coords
            0.3f,  1.0f, 0.0f,      1.0f, 1.0f, 1.0f,       1.0f, 0.0f,   // top right
            0.3f, 0.5f, 0.0f,       1.0f, 1.0f, 1.0f,       1.0f, 1.0f,   // bottom right
            -0.3f, 0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       0.0f, 1.0f,   // bottom left
            -0.3f,  1.0f, 0.0f,     1.0f, 1.0f, 1.0f,       0.0f, 0.0f    // top left 
        };

        unsigned int indices[] = {  
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        unsigned int VBO, EBO;
        glGenVertexArrays(1, &vertexArray);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        std::cout << "BEGIN loading shaders." << std::endl;

        shaderProgram = new SKEngio::ShaderProgram();
        shaderProgram->LoadShader("./effects/vShaderTHREE.txt", SKEngio::ShaderProgram::VERTEX);
        shaderProgram->LoadShader("./effects/fShaderTHREE.txt", SKEngio::ShaderProgram::FRAGMENT);
        shaderProgram->CreateProgram();

        std::cout << "END loading shaders." << std::endl;

        texture = new SKEngio::TextureLoader();
        texture->Load("./resources/textures/amiga.jpg", false);

    }

    void OnDetach() {
        //glDeleteShader(vertexShader);
        //glDeleteShader(fragmentShader); 

        delete shaderProgram;
        delete texture;

        glDeleteVertexArrays(1, &vertexArray);

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {
               
    }

    void OnUpdate(float t) {

    }

    void OnDraw(float t) {

        texture->bind();
        shaderProgram->Use();

        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    
};