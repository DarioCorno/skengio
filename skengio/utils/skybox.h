#pragma once

#ifndef SK_SKYBOX_
#define SK_SKYBOX_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <skengio/utils/textureManager.h>

namespace SKEngio {
	class SKYBox {
        public:
		    SKYBox(const std::vector<std::string>& facesFiles) {

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
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);

                shader = std::make_unique<ShaderProgram>();
                shader->LoadShader("./shaders/", "skybox.vert", SKEngio::ShaderProgram::VERTEX);
                shader->LoadShader("./shaders/", "skybox.frag", SKEngio::ShaderProgram::FRAGMENT);
                shader->CreateProgram();

                loadTextures(facesFiles);

                shader->SetCubeTexture(cubemapTexture->textureUnit);
		    }

            // prevent copying object
            SKYBox(const SKYBox&) = delete;
            SKYBox(SKYBox&&) = delete;
            SKYBox& operator=(const SKYBox&) = delete;
            SKYBox& operator=(SKYBox&&) = delete;

            ~SKYBox() {
                glDeleteVertexArrays(1, &skyboxVAO);
                glDeleteBuffers(1, &skyboxVBO);
            }

            void render(Camera* camera) {

                // draw skybox as last
                glEnable(GL_CULL_FACE);
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_TEXTURE_2D);
                // skybox is rendered at the same depth as glClearDepth value
                // change depth function so depth test passes when values are equal to depth buffer's content
                glDepthFunc(GL_LEQUAL);  

                shader->bind();
                shader->SetCameraUniforms(camera);
                //WARNING! This strips the translation from the camera matrix
                glm::mat4 viewNoTrans = glm::mat4(glm::mat3(camera->getViewMatrix() ));
                shader->SetViewMatrix(viewNoTrans);

                cubemapTexture->bind();

                // render the skybox cube
                glBindVertexArray(skyboxVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                cubemapTexture->unbind();

                glDepthFunc(GL_LESS); // set depth function back to default

            }

            std::unique_ptr<ShaderProgram> shader;
            unsigned int skyboxVAO, skyboxVBO;
            std::unique_ptr<Texture> cubemapTexture;

            private:

                bool loadTextures(const std::vector<std::string>& facesFiles) {
                    cubemapTexture = TextureManager::getInstance().LoadCubemap(facesFiles);
                    return cubemapTexture != nullptr;
                }

	};
}

#endif 