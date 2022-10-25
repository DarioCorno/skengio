#include "scene.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "skengio/window.h"

namespace SKEngio {

    Scene::Scene(unsigned int sceneid) {
        //initializes the layers manager
        sceneID = sceneid;
    };

    Scene::~Scene() {
        delete camera;
        delete music;
        for (Layer* layer : layers) {
            layer->OnDetach();
        }
        layers.clear();
    };

    void Scene::OnAttach() {
        //runs when attached to parent
    }

    void Scene::OnDetach() {
        //runs when detached (destroyed too)
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

        //pass the event to layers
        for (Layer* layer : layers) {
            layer->OnEvent(e);
        }

    }

    void Scene::OnDrawGUI(RenderParams* rp) {
        if(!enabled) 
            return;

        for(Layer* layer : layers) {
            if(layer->enabled) {
                layer->OnDrawGUI(rp);
            }
        }
    }

    void Scene::UpdateAndDraw(RenderParams* rp) {
        
        if(!enabled) 
            return;

        music->updateFFT();

        //update and render all scenes
        for(Layer* layer : layers) {
            if(layer->enabled) {
                layer->setCamera( camera );
                layer->OnUpdate(rp);
                layer->OnDraw(rp);
            }
        }        
    }

    void Scene::PushLayer(Layer* layer) {
        layer->setCamera( camera );
        layer->OnAttach();
        layers.push_back(layer);
    }

    Light* Scene::AddLight() {
        Light* newL = new Light();
        lights.push_back(newL);
        return newL;
    }

    Camera* Scene::SetCamera(float fov, std::string camID) {
        camera =  new Camera(
            WindowManager::get().width,
            WindowManager::get().height, fov, std::move(camID) );

        return camera;
    }

    void Scene::handleResize(int width, int height) {
        camera->handleResize(width, height);
    }

}