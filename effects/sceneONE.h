#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "skengio/scene.h"
#include "skengio/entities/geometries/torus.h"
#include "skengio/entities/geometries/plane.h"
#include "skengio/utils/skybox.h"
#include "skengio/logger.h"
#include "skengio/renderer.h"

namespace SKEngio {

	class SceneONE final : public Scene {

		SKEngio::Entity* torus;
		SKEngio::Entity* plane;
		SKEngio::SKYBox* sky;
		SKEngio::Light* light;

        void OnAttach() override {
            SK_LOG("Attaching scene " << GetName());

            //moved here for debug
            std::vector<std::string> faces
            {
                "resources/textures/skybox2/posx.jpg",
                "resources/textures/skybox2/negx.jpg",
                "resources/textures/skybox2/posy.jpg",
                "resources/textures/skybox2/negy.jpg",
                "resources/textures/skybox2/posz.jpg",
                "resources/textures/skybox2/negz.jpg"
            };
            sky = new SKEngio::SKYBox(faces);

            plane = NewEntity();
            plane->mesh = new SKEngio::Plane();
            ((SKEngio::Plane*)plane->mesh)->Generate(40.0f, 40.0f, 4, 4);
            plane->mesh->buildInterleavedArray();
            plane->mesh->createGLBuffers();

            plane->shader = new SKEngio::ShaderProgram();
            plane->shader->LoadShader("./shaders/", "basicshader.vert", SKEngio::ShaderProgram::VERTEX);
            plane->shader->LoadShader("./shaders/", "basicshader.frag", SKEngio::ShaderProgram::FRAGMENT);
            plane->shader->CreateProgram();

            plane->material->diffuseTexture = SKEngio::TextureManager::get().Load("./resources/textures/checker.jpg", false);
            plane->translate(0.0f, -8.0f, 0.0f);
            plane->rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            plane->castsShadows = false;
            plane->updateSelfAndChild();    //planeis still, non need to update transforms
            plane->shader->SetMaterialUniforms(plane->material); //plane material is static

            torus = NewEntity();
            torus->mesh = new SKEngio::Torus();
            ((SKEngio::Torus*)torus->mesh)->Generate(2.0f, 6.0f, 32, 24, M_PI * 2.0f);  //torus
            torus->mesh->buildInterleavedArray();
            torus->mesh->createGLBuffers();

            torus->shader = new SKEngio::ShaderProgram();
            torus->shader->LoadShader("./shaders/", "basicshader.vert", SKEngio::ShaderProgram::VERTEX);
            torus->shader->LoadShader("./shaders/", "basicshader.frag", SKEngio::ShaderProgram::FRAGMENT);
            torus->shader->CreateProgram();

            torus->material->diffuseTexture = SKEngio::TextureManager::get().Load("./resources/textures/metal.jpg", false);
            torus->setCubemap(sky->cubemapTexture);

            //create a light with defaults
            light = NewLight();
            light->initGizmo( SKEngio::Renderer::get().GizmoGetShader() );

        }

        void OnDetach() override {

            delete sky;

            SK_LOG("Destroying Scene " << this->GetName());
        }

        void OnDrawGUI(SKEngio::RenderParams* rp) override {
            ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Effect TWO");
            ImGui::Text("Ambient");
            ImGui::ColorEdit3("ambient", (float*)glm::value_ptr(torus->material->materialAmbientColor));
            ImGui::Text("Diffuse");
            ImGui::ColorEdit3("diffuse", (float*)glm::value_ptr(torus->material->materialDiffuseColor));
            ImGui::Text("Specular:");
            ImGui::ColorEdit3("specular", (float*)glm::value_ptr(torus->material->materialSpecularColor));
            ImGui::Text("Shininess:");
            ImGui::SliderFloat("shininess", &torus->material->materialShininess, 0.0, 128.0);
            ImGui::Text("Reflectivity:");
            ImGui::SliderFloat("reflectivity", &torus->material->materialReflectivity, 0.0f, 1.0f);
            ImGui::End();
        }

        void OnUpdate(SKEngio::RenderParams* rp) override {

            float t = rp->time;

            torus->translate(0.0f, 0.0f, (sin(t / 2.0f) * 10.0f));
            torus->rotate((float)(t * 50.0f), 0.5f, 0.5f, 0.0f);

            glm::vec3 pos = rp->camera->position;
            pos.x = sin(t / 10.0f) * 50.0f;
            pos.z = cos(t / 10.0f) * 50.0f;
            rp->camera->setPosition(pos);

            light->SetPosition(sin(t) * 10.0f, 15.0f, -3.0f);
            //update the transform matrices (plane is fixed)
            light->updateSelfAndChild();
            torus->updateSelfAndChild();

            glm::vec3 lPos = light->GetPosition();
            glm::vec3 lDiff = light->GetDiffuse();
            glm::mat4 lVPMat = light->getLightViewProjMatrix();
            torus->shader->SetLightUniforms(lPos, lDiff, lVPMat);
            plane->shader->SetLightUniforms(lPos, lDiff, lVPMat);

            torus->shader->SetMaterialUniforms(torus->material);    //torus materials can be changed at runtime

        }

        void OnDraw(SKEngio::RenderParams* rp) override {
            Scene::OnDraw(rp);

            //return;
            //
            //glEnable(GL_TEXTURE_2D);
            //glEnable(GL_DEPTH_TEST);
            //
            //
            //torus->render(rp);
            //plane->render(rp);
            //
            //light->renderGizmo(rp);

            //skybox is rendered as last not to waste fragments (see its render function for details)
            sky->render(rp->camera);

        }

	};
}
