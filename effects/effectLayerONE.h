#pragma once

#include "../skengio/core.h"

class EffectOne : public SKEngio::Layer {
    public:

    unsigned int vertexArray, vertexBuffer, indexBuffer;
    unsigned int vertexShader, fragmentShader, shaderProgram;
    float fxparam = 1.0f;

    void OnAttach() {
        std::cout << "Layer attached, creating buffers " << this->GetId() << std::endl;

        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        float vertices[3 * 3] = {
            -0.5f,  -0.25f, 0.0f,
            0.0f, -0.25f, 0.0f,
            -0.25f, 0.25f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //create the vertex shader
        //shader source
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "out vec4 vertexColor;\n"  //colro sent from vertex shader to fragment shader
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "vertexColor = gl_Position;\n"
            "}\0";        

        //create a shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        //set the shader source
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);        
        glCompileShader(vertexShader);        

        //check if shader was compiled correctly
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);        
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }        

        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec4 vertexColor;"   //color received from vertex shader
            "uniform vec4 ourColor;\n"  //variable that we will update form C code
            "void main()\n"
            "{\n"
            "    FragColor = ourColor;\n"  //vertexColor; //vec4(1.0f, 0.5f, 0.2f, 1.0f);
            "}\0;";
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);      
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }


        //create the shaderProgram (vertex + fragment)
        shaderProgram = glCreateProgram();          
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);        

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }        


    }

    void OnDetach() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); 

        glDeleteVertexArrays(1, &vertexArray);

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {

        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver );
        ImGui::SetNextWindowBgAlpha(0.5f);

        ImGui::Begin("Effect ONE");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Flash speed:");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &fxparam, 0.1f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        ImGui::End();                    
    }

    void OnUpdate(float t) {

        glUseProgram(shaderProgram);        
        float greenValue = sin(t * fxparam) / 2.0f + 0.5f;
        //retrieve the address of the shader variable "ourColor"
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //set the value of shader variable ourColor
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    }

    void OnDraw(float t) {

        glUseProgram(shaderProgram);        

        /* float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //retrieve the address of the shader variable "ourColor"
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //set the value of shader variable ourColor
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); */

        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    
};