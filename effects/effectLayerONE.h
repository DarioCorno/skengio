#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "skengio/defines.h"
#include "skengio/utils/shaderProgram.h"
#include <skengio/layer.h>
#include "skengio/renderparams.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class EffectOne final : public SKEngio::Layer {
    public:

        unsigned int vertexArray{};
        unsigned int vertexBuffer{};
        unsigned int indexBuffer{};

        float fxparam = 1.0f;
        SKEngio::ShaderProgram* shaderProgram;
        bool useLoader = true;
        int vertexColorLocation = 0;

        int projMatrixLocation = 0;
        int viewMatrixLocation = 0;
        int modelMatrixLocation = 0;


        void OnAttach() override {

            glGenVertexArrays(1, &vertexArray);
            glBindVertexArray(vertexArray);

            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

            float vertices[] = {
                // positions        
                1.0f,  -1.0f, 0.0f,  
                -1.0f, -1.0f, 0.0f,  
                0.0f, 1.0f, 0.0f, 
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            unsigned int indices[3] = { 0, 1, 2 };
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            shaderProgram = new SKEngio::ShaderProgram();
            shaderProgram->LoadShader("./effects/", "vShaderONE.txt", SKEngio::ShaderProgram::VERTEX);
            shaderProgram->LoadShader("./effects/", "fShaderONE.txt", SKEngio::ShaderProgram::FRAGMENT);
            shaderProgram->CreateProgram();

            //retrieve the address of the shader uniforms
            vertexColorLocation = glGetUniformLocation(shaderProgram->programID , "vertexColor");

            projMatrixLocation = glGetUniformLocation(shaderProgram->programID, PROJ_MATRIX_UNIFORM_NAME );
            viewMatrixLocation = glGetUniformLocation(shaderProgram->programID, VIEW_MATRIX_UNIFORM_NAME );
            modelMatrixLocation = glGetUniformLocation(shaderProgram->programID, MODEL_MATRIX_UNIFORM_NAME );

        }

        void OnDetach() override {

            delete shaderProgram;
            glDeleteVertexArrays(1, &vertexArray);

        }

        void OnDrawGUI(SKEngio::RenderParams* rp) override {

            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver );
            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Effect ONE");
            ImGui::Text("Flash speed:");
            ImGui::SliderFloat("float", &fxparam, 0.1f, 20.0f);
            ImGui::Text("ShaderProg ID: %i", shaderProgram->programID);
            ImGui::Text("Camera     ID: %s", rp->camera->id.c_str());
            ImGui::End();                    
        }

        void OnUpdate(SKEngio::RenderParams* rp) override {

            float t = rp->time;

            float greenValue = sin(t * fxparam) / 2.0f + 0.5f;

            glProgramUniform4f( shaderProgram->programID, vertexColorLocation, 0.0f, greenValue, 0.0f, 0.0f );

            //model matrix, identity
            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::rotate( model, (t * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            //set the shader uniform matrices
            glProgramUniformMatrix4fv( shaderProgram->programID, projMatrixLocation, 1, GL_FALSE, glm::value_ptr( rp->camera->getProjMatrix() ) );
            glm::mat4 viewMat = rp->camera->getViewMatrix();
            glProgramUniformMatrix4fv( shaderProgram->programID, viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat) );
            glProgramUniformMatrix4fv( shaderProgram->programID, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model) );
        }

        void OnDraw(SKEngio::RenderParams* rp) override {

            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
        
            shaderProgram->bind();

            glBindVertexArray(vertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
            glEnable(GL_CULL_FACE);
        }

    
};