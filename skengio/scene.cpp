#include "scene.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "skengio/window.h"
#include "materialsManager.h"

namespace SKEngio {

    std::string Scene::GetName() {
        return dispName;
    }

    void Scene::OnAttach() {
        //runs when attached to parent
    }

    void Scene::OnDetach() {
        //runs when detached (destroyed too)
        delete camera;
        delete music;

        //detach and destroy all entities
        for (Entity* ent : entities) {
            ent->OnDetach();
            delete ent;
        }

        //detach and delete all lights
        for (Light* lt : lights) {
            lt->OnDetach();
            delete lt;
        }

        SK_LOG("Detaching scene: " << dispName);
    }    

    void Scene::OnEvent(Event* e) {

        if (e->type == EventType::KeyPress) {
            if (e->key == GLFW_KEY_W) {
                camera->moveForward(0.1f);
            }
            if (e->key == GLFW_KEY_S) {
                camera->moveForward(-0.1f);
            }
            if (e->key == GLFW_KEY_A) {
                camera->translate(0.1f, 0.0f, 0.0f);
            }
            if (e->key == GLFW_KEY_D) {
                camera->translate(-0.1f, 0.0f, 0.0f);
            }
            if (e->key == GLFW_KEY_Q) {
                camera->translate(-0.1f, 0.0f, 0.0f);
            }
        }

        //pass the event to entities (I would say no, the fx scene manages events)?
        //for (Entity* ent : entities) {
        //    ent->OnEvent(e);
        //}

    }

    void Scene::OnDrawGUI(RenderParams* rp) {
        if(!enabled) 
            return;

        //for(Layer* layer : layers) {
        //    if(layer->enabled) {
        //        layer->OnDrawGUI(rp);
        //    }
        //}
    }

    void Scene::OnUpdate(RenderParams* rp) {
        
        if(!enabled) 
            return;


        int lIdx = 0;
        for (Light* light : lights) {
            //update transform
            light->updateSelfAndChild();

            glm::vec3 lPos = light->GetPosition();
            glm::vec3 lDiff = light->GetDiffuse();
            glm::mat4 lVPMat = light->getLightViewProjMatrix();

            for (Entity* ent : entities) {
                //update transform
                ent->updateSelfAndChild();

                //update lights to ent materials
                ShaderProgram* entShader = ent->material->GetShader();
                entShader->SetVec3("pointLights[" + std::to_string(lIdx) + "].lightPosition", lPos);
                entShader->SetVec3("pointLights[" + std::to_string(lIdx) + "].lightDiffuse", lDiff);
            }
            lIdx++;
        }




        music->updateFFT();

    }


    void Scene::OnDraw(RenderParams* rp) {

        if (!enabled)
            return;

        //render all entities
        for (Entity* ent : entities) {
            ent->render(rp);
        }

        if (rp->pass == RenderPass::Final) {
            //render all lights
            for (Light* lt : lights) {
                lt->renderGizmo(rp, lt->GetDiffuse() );
            }
        }

    }
    //void Scene::PushLayer(Layer* layer) {
    //    layer->OnAttach();
    //    layers.push_back(layer);
    //}

    Light* Scene::NewLight() {
        Light* newL = new Light();
        lights.push_back(newL);
        return newL;
    }

    Entity* Scene::NewEntity() {
        Entity* newE = new Entity();
        entities.push_back(newE);
        return newE;
    }

    Camera* Scene::SetCamera(float fov, std::string camID) {
        if (camera != nullptr)
            delete camera;

        camera =  new Camera(
            WindowManager::get().width,
            WindowManager::get().height, 
            fov, 
            std::move(camID) );

        camera->setPosition(0.0f, 0.0f, 30.0f);
        camera->setTarget(0.0f, 0.0f, 0.0f);

        return camera;
    }

    void Scene::handleResize(int width, int height) {
        camera->handleResize(width, height);
    }

}