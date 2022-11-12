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
        SKEngio::Light* light2;

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

            //create a light with defaults and defines for shaders
            light = NewLight();
            light->SetDiffuse(0.3f, 0.3f, 0.3f);
            light->SetSpecular(0.8f, 0.8f, 0.8f);
            light->SetAmbient(0.2f, 0.2f, 0.2f);
            light->lightType = LightType::DirectionalLight;
            light->initGizmo(SKEngio::Renderer::get().GizmoGetShader() );
            light->GenerateDirShadowMapBuffer(Renderer::get().GetShadowMapFBOID(), 1024, 1024);

            light->SetPosition(12.0f, 12.0f, 0.0f);
            light->rotate(0.0f, 90.0f, 0.0f);
            light->rotate(-45.0f, 0.0f, 0.0f);

            light2 = NewLight();
            light2->SetDiffuse(0.4f, 0.0f, 0.0f);
            light2->SetSpecular(0.6f, 0.2f, 0.2f);
            light2->castShadows = false;
            light2->initGizmo(SKEngio::Renderer::get().GizmoGetShader() );
            light2->GenerateDirShadowMapBuffer(Renderer::get().GetShadowMapFBOID(), 1024, 1024);

            //all shaders contain some #willdefine pseudo directives, it will be filled with the following values
            std::list<ShaderDefine> defines;
            ShaderDefine numPLights = { "NUM_LIGHTS", std::to_string( lights.size() ) };
            defines.push_back(numPLights);

            plane = NewEntity("Plane");
            plane->mesh = new SKEngio::Plane();
            ((SKEngio::Plane*)plane->mesh)->Generate(40.0f, 40.0f, 4, 4);

            plane->material->LoadShader("./shaders/", "basicshader.vert", SKEngio::ShaderProgram::VERTEX, defines);
            plane->material->LoadShader("./shaders/", "basicshader.frag", SKEngio::ShaderProgram::FRAGMENT, defines);
            plane->material->CreateProgram();

            plane->material->diffuseTexture = SKEngio::TextureManager::get().Load("./resources/textures/checker.jpg", false);
            plane->rotate(-90.0f, 0.0f, 0.0f);
            plane->castsShadows = false;
            plane->updateSelfAndChild();    //plane is still, no need to update transforms in the main loop

            torus = NewEntity("Torus");
            torus->mesh = new SKEngio::Torus();
            ((SKEngio::Torus*)torus->mesh)->Generate(2.0f, 6.0f, 32, 24, M_PI * 2.0f);  //torus
            torus->mesh->createGLBuffers();

            torus->material->LoadShader("./shaders/", "basicshader.vert", SKEngio::ShaderProgram::VERTEX, defines);
            torus->material->LoadShader("./shaders/", "basicshader.frag", SKEngio::ShaderProgram::FRAGMENT, defines);
            torus->material->CreateProgram();

            torus->material->diffuseTexture = SKEngio::TextureManager::get().Load("./resources/textures/metal.jpg", false);
            //torus->material->specularTexture = SKEngio::TextureManager::get().Load("./resources/textures/checker.jpg", false);
            //torus->material->useSpecularTexture = 1;
            torus->material->SetCubemap(sky->cubemapTexture);

        }

        void OnDetach() override {

            delete sky;

            Scene::OnDetach();

            SK_LOG("Destroying Scene " << this->GetName());
        }

        void OnDrawGUI(SKEngio::RenderParams* rp) override {
            //ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
            //ImGui::SetNextWindowBgAlpha(0.5f);
            
            //ImGui::Begin("Material 01");
            //ImGui::Text("Ambient");
            //ImGui::ColorEdit3("ambient", (float*)glm::value_ptr(torus->material->materialAmbientColor));
            //ImGui::Text("Diffuse");
            //ImGui::ColorEdit3("diffuse", (float*)glm::value_ptr(torus->material->materialDiffuseColor));
            //ImGui::Text("Specular:");
            //ImGui::ColorEdit3("specular", (float*)glm::value_ptr(torus->material->materialSpecularColor));
            //ImGui::Text("Shininess:");
            //ImGui::SliderFloat("shininess", &torus->material->materialShininess, 0.0, 128.0);
            //ImGui::Text("Reflectivity:");
            //ImGui::SliderFloat("reflectivity", &torus->material->materialReflectivity, 0.0f, 1.0f);

            //ImGui::End();
        }

        void OnUpdate(SKEngio::RenderParams* rp) override {

            float t = rp->time;

            torus->setPosition(0.0f, 8.0f, (sin(t / 2.0f) * 10.0f));
            torus->rotate(rp->deltaTime * 5.0f, 0.0f, 0.0f);

            //glm::vec3 pos = rp->camera->position;
            //pos.x = sin(t / 10.0f) * 50.0f;
            //pos.y = 30.0f;
            //pos.z = cos(t / 10.0f) * 50.0f;
            //rp->camera->setPosition(pos);

            //light->SetPosition(sin(t) * 10.0f, 15.0f, -3.0f);

            light2->SetPosition(cos(t) * 14.0f, 12.0f, sin(t) * 14.0f);

            Scene::OnUpdate(rp);
        }

        void OnDraw(SKEngio::RenderParams* rp) override {
            Scene::OnDraw(rp);

            //skybox is rendered as last not to waste fragments (see its render function for details)
            sky->render(rp->camera);
        }

	};
}
