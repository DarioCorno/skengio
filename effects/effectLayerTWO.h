#pragma once

#include "../skengio/core.h"

class EffectTwo : public SKEngio::Layer {
    public:

    unsigned int VBO, VAO;
    unsigned int vertexShader, fragmentShader, shaderProgram;
    glm::mat4 transform = glm::mat4(1.0f);
    unsigned int transformUniformLocation;

    void OnAttach() {
        std::cout << "Layer attached, creating buffers " << this->GetId() << std::endl;


        const char *vertexShaderSource ="#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 ourColor;\n"
            "uniform mat4 transform;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = transform * vec4(aPos, 1.0f);\n"
            "   //gl_Position = vec4(aPos, 1.0);\n"
            "   ourColor = aColor;\n"
            "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 ourColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(ourColor, 1.0f);\n"
            "}\n\0";

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions         // colors
            0.2f, -0.1f, 0.0f,  1.0f, 1.0f, 0.0f,  // bottom right
            0.1f, 0.1f, 0.0f,  1.0f, 0.0f, 1.0f,   // top 
            0.0f, -0.1f, 0.0f,  0.0f, 1.0f, 1.0f  // bottom left
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // glBindVertexArray(0);

        // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
        glUseProgram(shaderProgram);
        std::cout << "Layer TWO Program " << shaderProgram << std::endl;

        transformUniformLocation = glGetUniformLocation(shaderProgram, "transform");

    }

    void OnDetach() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); 
        glDeleteProgram(shaderProgram);        

        std::cout << "Destroy Layer " << this->GetId() << std::endl;
    }

    void OnDrawGUI(float t) {

    }

    void OnUpdate(float t) {
        //OpenGL 4.1 specs (4.5 specs doesn't need to use the program before setting uniforms)
        //glUseProgram(shaderProgram);        
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)(t * 80.0f), glm::vec3(0.0f, 1.0f, 0.0f));        
        transform = glm::translate(transform, glm::vec3(-0.1f, 0.0f, 0.0f));
        //glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));        
        glProgramUniformMatrix4fv(shaderProgram, transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));
    }

    void OnDraw(float t) {
        glDisable(GL_CULL_FACE);

        glUseProgram(shaderProgram);        

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    
};