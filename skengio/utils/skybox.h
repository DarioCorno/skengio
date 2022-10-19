#pragma once

#ifndef _SK_SKYBOX_
#define _SK_SKYBOX_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <skengio/utils/textureManager.h>

namespace SKEngio {
	class SKYBox {
        public:
		    SKYBox() {
                float skyboxVertices[] = {
                    // positions          
                    -1.0f,  1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    -1.0f,  1.0f, -1.0f,
                     1.0f,  1.0f, -1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f
                };

                glGenVertexArrays(1, &skyboxVAO);
                glGenBuffers(1, &skyboxVBO);
                glBindVertexArray(skyboxVAO);
                glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

                shader = new ShaderProgram();
                shader->LoadShader("./shaders/", "skybox.vert", SKEngio::ShaderProgram::VERTEX);
                shader->LoadShader("./shaders/", "skybox.frag", SKEngio::ShaderProgram::FRAGMENT);
                shader->CreateProgram();
		    }

            ~SKYBox() {
                glDeleteVertexArrays(1, &skyboxVAO);
                glDeleteBuffers(1, &skyboxVBO);
                delete cubemapTexture;
                delete shader;
            }

            bool loadTextures(std::vector<std::string> facesFiles, bool freeData) {
                cubemapTexture = SKEngio::TextureManager::getInstance()->LoadCubemap(facesFiles, false);
                return cubemapTexture;
            }

            void render(Camera* camera) {
                // draw skybox as last
                glEnable(GL_CULL_FACE);
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
                // skybox is rendered at the same depth as glClearDepth value
                // change depth function so depth test passes when values are equal to depth buffer's content
                glDepthFunc(GL_LEQUAL);  

                shader->bind();
                shader->SetCameraUniforms(camera);
                //WARNING! This strips the translation from the camera matrix
                glm::mat4 viewNoTrans = glm::mat4(glm::mat3(camera->getViewMatrix() ));
                shader->SetViewMatrix(viewNoTrans);


                // skybox cube
                glBindVertexArray(skyboxVAO);
                //glActiveTexture(GL_TEXTURE11);
                cubemapTexture->bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS); // set depth function back to default

                cubemapTexture->unbind();
            }

            ShaderProgram* shader;
            unsigned int skyboxVAO, skyboxVBO;
            TextureLoader* cubemapTexture;

	};
}

#endif 