#pragma once

#include <skengio/utils/object.h>
#include <skengio/utils/skybox.h>
#include <skengio/utils/geometries/mesh.h>
#include <skengio/utils/geometries/torus.h>
#include <skengio/utils/geometries/box.h>
#include <skengio/utils/geometries/plane.h>
#include <skengio/utils/textureManager.h>
#include <skengio/renderparams.h>

class EffectTwo : public SKEngio::Layer {
    public:

        glm::mat4 model = glm::mat4(1.0f);  //model matrix

        //SKEngio::Sphere* mesh;
        SKEngio::Object* torus;
        SKEngio::Object* plane;
        SKEngio::SKYBox* sky;
        SKEngio::Light* light;

        void OnAttach() {
            SK_LOG("Attaching layer " << this->GetId() );

            //moved here for debug
            std::vector<std::string> faces
            {
                "resources/textures/skybox/posx.jpg",
                "resources/textures/skybox/negx.jpg",
                "resources/textures/skybox/posy.jpg",
                "resources/textures/skybox/negy.jpg",
                "resources/textures/skybox/posz.jpg",
                "resources/textures/skybox/negz.jpg"
            };
            sky = new SKEngio::SKYBox(faces);

            plane = new SKEngio::Object();
            plane->mesh = new SKEngio::Plane();
            ((SKEngio::Plane*)plane->mesh)->Generate(20.0f, 20.0f, 4, 4);  
            plane->mesh->buildInterleavedArray();
            plane->mesh->createGLBuffers();

            plane->shader = new SKEngio::ShaderProgram();
            plane->shader->LoadShader("./shaders/", "basicshader.vert", SKEngio::ShaderProgram::VERTEX);
            plane->shader->LoadShader("./shaders/", "basicshader.frag", SKEngio::ShaderProgram::FRAGMENT);
            plane->shader->CreateProgram();

            plane->material->diffuseTexture = SKEngio::TextureManager::getInstance()->Load("./resources/textures/checker.jpg", false);
            //plane is still, we can set his model matrix now
            plane->translate(0.0f, -2.0f, 0.0f);
            plane->rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            plane->castsShadows = false;

            torus = new SKEngio::Object();
            torus->mesh = new SKEngio::Torus();
            ( (SKEngio::Torus*)torus->mesh )->Generate(0.4f, 1.0f, 32, 24, M_PI * 2.0f);  //torus
            torus->mesh->buildInterleavedArray();
            torus->mesh->createGLBuffers();

            torus->shader = new SKEngio::ShaderProgram();
            torus->shader->LoadShader("./shaders/","basicshader.vert", SKEngio::ShaderProgram::VERTEX);
            torus->shader->LoadShader("./shaders/","basicshader.frag", SKEngio::ShaderProgram::FRAGMENT);
            torus->shader->CreateProgram();

            torus->material->diffuseTexture = SKEngio::TextureManager::getInstance()->Load("./resources/textures/metal.jpg", false);
            //create a light with deafult colors
            light = new SKEngio::Light();

            //move camera position and target, camera was set from renderer activeCamera
            activeCamera->setPosition(0.0f, 0.0f, -4.0f);
            activeCamera->setTarget(0.0f, 0.0f, 0.0f);

        }

        void OnDetach() {
            delete torus;
            delete plane;
            delete light;
            delete sky;

            SK_LOG("Destroying Layer " << this->GetId() );
        }

        void OnDrawGUI(SKEngio::RenderParams* rp) {
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

        void OnUpdate(SKEngio::RenderParams* rp) {

            float t = rp->time;

            torus->resetTransforms();
            torus->translate(0.0f, 0.0f, (sin(t / 2) * 5.0f));
            torus->rotate((float)(t * 50.0f), 0.5f, 0.5f, 0.0f);

            glm::vec3 pos = activeCamera->position;
            pos.x = sin(t / 10.0f) * 10.0f;
            pos.z = cos(t / 10.0f) * 10.0f;
            activeCamera->setPosition(pos);

            //set the shader camera uniforms
            torus->shader->SetCameraUniforms(activeCamera);
            plane->shader->SetCameraUniforms(activeCamera);

            light->SetPosition(-10.0f, 20.0f, -10.0f);

            torus->shader->SetLightUniforms(light);
            torus->shader->SetMaterialUniforms(torus->material);
            plane->shader->SetLightUniforms(light);
            plane->shader->SetMaterialUniforms(plane->material);

        }

        void OnDraw(SKEngio::RenderParams* rp) {

            glEnable(GL_TEXTURE_2D);
            glEnable(GL_DEPTH_TEST);


            torus->render(rp);
            plane->render(rp);


            //skybox is rendered as last not to waste fragments (see the render function for details)
            if(! (rp->pass == SKEngio::RenderPass::ShadowDepth))
                sky->render(activeCamera);

        }

    
};