#pragma once

#include "../skengio/core.h"

class EffectTwo : public SKEngio::Layer {
    public:

    unsigned int VBO, VAO;
    unsigned int vertexShader, fragmentShader, shaderProgramID;
    glm::mat4 model = glm::mat4(1.0f);  //model matrix
    unsigned int projMatrixLocation;
    unsigned int viewMatrixLocation;
    unsigned int modelMatrixLocation;
    SKEngio::Sphere* mesh;

    void OnAttach() {
        std::cout << "Layer attached, creating buffers " << this->GetId() << std::endl;


        const char *vertexShaderSource ="#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "layout (location = 2) in vec3 aNormal;\n"
            "layout (location = 3) in vec2 aText;\n"
            "out vec4 ourColor;\n"
            "uniform mat4 projMatrix;\n"
            "uniform mat4 viewMatrix;\n"
            "uniform mat4 modelMatrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);\n"
            "   ourColor = aColor;\n"
            "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec4 ourColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = ourColor;\n"
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
        shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);
        glLinkProgram(shaderProgramID);
        // check for linking errors
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        /* ORIGINAL CODE
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
        glUseProgram(shaderProgramID);
        std::cout << "Layer TWO Program " << shaderProgramID << std::endl;
        */

       /*
        mesh = new SKEngio::Mesh();
        mesh->addVertex(0.3f, -0.1f, 0.0f);
        mesh->addVertex(0.3f, 0.2f, 0.0f);
        mesh->addVertex(0.1f, 0.2f, 0.0f);
        mesh->addVertex(0.1f, -0.1f, 0.0f);

        mesh->addColor(0.0f, 1.0f, 1.0f, 1.0f);
        mesh->addColor(1.0f, 1.0f, 0.0f, 1.0f);
        mesh->addColor(1.0f, 0.0f, 1.0f, 1.0f);
        mesh->addColor(1.0f, 1.0f, 0.0f, 1.0f);

        mesh->addTexCoord(0.0f , 0.0f);
        mesh->addTexCoord(1.0f , 0.0f);
        mesh->addTexCoord(0.0f , 1.0f);
        mesh->addTexCoord(1.0f , 0.0f);

        mesh->addNormal(0.0f, 1.0f, 0.0f);
        mesh->addNormal(0.0f, 1.0f, 0.0f);
        mesh->addNormal(1.0f, 1.0f, 0.0f);
        mesh->addNormal(0.0f, 1.0f, 0.0f);

        mesh->addTriIndices(0, 1, 2);
        mesh->addTriIndices(0, 2, 3);

        mesh->buildInterleavedArray();

        mesh->createGLBuffers();
        */

        mesh = new SKEngio::Sphere();
        mesh->Generate(1.0f, 12, 12);
        mesh->buildInterleavedArray();
        mesh->createGLBuffers();

        std::cout << "Layer TWO Program " << shaderProgramID << std::endl;
        glUseProgram(shaderProgramID);
        projMatrixLocation = glGetUniformLocation(shaderProgramID, PROJ_MATRIX_UNIFORM_NAME );
        viewMatrixLocation = glGetUniformLocation(shaderProgramID, VIEW_MATRIX_UNIFORM_NAME );
        modelMatrixLocation = glGetUniformLocation(shaderProgramID, MODEL_MATRIX_UNIFORM_NAME );

    }

    void OnDetach() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); 
        glDeleteProgram(shaderProgramID);        

        delete mesh;        

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
        glProgramUniformMatrix4fv( shaderProgramID, projMatrixLocation, 1, GL_FALSE, glm::value_ptr( activeCamera->getProjMatrix() ) );
        glm::mat4 viewMat = activeCamera->getViewMatrix();
        glProgramUniformMatrix4fv( shaderProgramID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat) );
        glProgramUniformMatrix4fv( shaderProgramID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model) );
    }

    void OnDraw(float t) {
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_ONE, GL_ONE);

        glUseProgram(shaderProgramID);        

        /*
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */
       mesh->draw();
    }

    
};