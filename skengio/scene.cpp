#include "scene.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace SKEngio {

    Scene::Scene(unsigned int sceneid) {
        //initializes the layers manager
        sceneID = sceneid;
    };

    Scene::~Scene() {
        delete music;
    };

    void Scene::OnAttach() {
        for(Layer* layer : layerStack.layers) {
            layer->OnAttach();
        }
    }

    void Scene::OnDetach() {
        for(Layer* layer : layerStack.layers) {
            layer->OnDetach();
        }
    }    

    void Scene::OnEvent(Event* e) {

        if (e->type == EventType::KeyPress) {
            if (e->key == GLFW_KEY_W) {
                activeCamera->moveForward(0.1f);
            }
            if (e->key == GLFW_KEY_S) {
                activeCamera->moveForward(-0.1f);
            }
            if (e->key == GLFW_KEY_A) {
                activeCamera->translate(0.1f, 0.0f, 0.0f);
            }
            if (e->key == GLFW_KEY_D) {
                activeCamera->translate(-0.1f, 0.0f, 0.0f);
            }
            if (e->key == GLFW_KEY_Q) {
                activeCamera->translate(-0.1f, 0.0f, 0.0f);
            }
        }
        //pass the event to layerStack
        layerStack.OnEvent(e);

    }

    void Scene::OnDrawGUI(RenderParams* rp) {
        if(!enabled) 
            return;

        for(Layer* layer : layerStack.layers) {
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
        for(Layer* layer : layerStack.layers) {
            if(layer->enabled) {
                layer->setCamera( activeCamera );
                layer->OnUpdate(rp);
                layer->OnDraw(rp);
            }
        }        
    }

    void Scene::PushLayer(Layer* layer) {
        layer->setCamera(activeCamera);
        layerStack.PushLayer(layer);
    }

    const LayerStack& Scene::GetLayerStack() const {
        return layerStack;
    }

    void Scene::setActiveCamera(Camera* cam) {
        activeCamera = cam;
    }

    void Scene::handleResize(int width, int height) {
        activeCamera->handleResize(width, height);
    }

}