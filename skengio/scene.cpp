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

        if (e->type == EventType::MouseMove) {
            xMouseDelta = e->xPos - xMousePos;
            yMouseDelta = e->yPos - yMousePos;

            xMousePos = e->xPos;
            yMousePos = e->yPos;

            if (lMousePressed) {
                //update camera rotation
                camera->rotate(xMouseDelta, yMouseDelta);
            }

            if (rMousePressed) {
                camera->strafe(-xMouseDelta);
                camera->rise(yMouseDelta);
            }

            if (mMousePressed) {
                camera->moveForward(yMouseDelta);
            }

        }

        if (e->type == EventType::MousePress) {
            if(e->button == 0)
                lMousePressed = true;

            if (e->button == 1)
                rMousePressed = true;

            if (e->button == 2)
                mMousePressed = true;
        }

        if (e->type == EventType::MouseRelease) {
            if (e->button == 0)
                lMousePressed = false;

            if (e->button == 1)
                rMousePressed = false;

            if (e->button == 2)
                mMousePressed = false;

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

        for (Entity* ent : entities) {
            //update transforms
            ent->updateSelfAndChild();
        }

        int lIdx = 0;
        for (Light* light : lights) {
            //update transform
            light->updateSelfAndChild();

            glm::vec3 lPos = light->GetPosition();
            glm::vec3 lDiff = light->GetDiffuse();
            glm::vec3 lAmb = light->GetAmbient();
            glm::vec3 lSpec = light->GetSpecular();
            glm::mat4 lVPMat = light->getDirLightViewProjMatrix();
            glm::vec3 lightDir = light->transform.getForward();

            //set light data for every entity
            for (Entity* ent : entities) {
                //update lights to ent materials
                ShaderProgram* entShader = ent->material->GetShader();
                entShader->SetMat4("lightViewProjMatrix[" + std::to_string(lIdx) + "]", lVPMat);
                entShader->SetInt("lights[" + std::to_string(lIdx) + "].lightType", (int)light->lightType);
                entShader->SetBool("lights[" + std::to_string(lIdx) + "].enabled", light->enabled);
                entShader->SetBool("lights[" + std::to_string(lIdx) + "].castShadows", light->castShadows);
                entShader->SetVec3("lights[" + std::to_string(lIdx) + "].lightPosition", lPos);
                entShader->SetVec3("lights[" + std::to_string(lIdx) + "].direction", lightDir );
                entShader->SetVec3("lights[" + std::to_string(lIdx) + "].lightDiffuse", lDiff);
                entShader->SetVec3("lights[" + std::to_string(lIdx) + "].lightAmbient", lAmb);
                entShader->SetVec3("lights[" + std::to_string(lIdx) + "].lightSpecular", lSpec);

                if(light->lightType == LightType::DirectionalLight && light->castShadows)
                    entShader->SetInt("lights[" + std::to_string(lIdx) + "].depthMap", light->GetShadowTexture()->textureUnit);  //dir light (same texture as point)

                if (light->lightType == LightType::PointLight && light->castShadows)
                    entShader->SetInt("lights[" + std::to_string(lIdx) + "].depthCubeMap", light->GetCubemapShadowTexture()->textureUnit);  //point light (same texture as dir)

                entShader->SetMat4("lights[" + std::to_string(lIdx) + "].lightViewProjMatrix", lVPMat);
                entShader->SetFloat("lights[" + std::to_string(lIdx) + "].constantAtt", light->constantAttenuation);
                entShader->SetFloat("lights[" + std::to_string(lIdx) + "].linearAtt", light->linearAttenuation);
                entShader->SetFloat("lights[" + std::to_string(lIdx) + "].quadraticAtt", light->quadraticAttenuation);
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
                if(lt->enabled)
                    lt->renderGizmo(rp, lt->GetDiffuse() );
            }
        }

    }

    Light* Scene::NewLight() {
        Light* newL = new Light();
        lights.push_back(newL);
        return newL;
    }

    Entity* Scene::NewEntity(std::string id) {
        Entity* newE = new Entity();
        newE->SetID(id);
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

        return camera;
    }

    void Scene::handleResize(int width, int height) {
        camera->handleResize(width, height);
    }

}