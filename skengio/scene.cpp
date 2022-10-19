#include "scene.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace SKEngio {

    Scene::Scene(unsigned int sceneid) {
        //initializes the layers manager
        layerStack = new LayerStack();
        sceneID = sceneid;
    };

    Scene::~Scene() {
        delete layerStack;
        delete music;
    };

    void Scene::OnAttach() {
        for(Layer* layer : layerStack->layers) {
            layer->OnAttach();
        }
    }

    void Scene::OnDetach() {
        for(Layer* layer : layerStack->layers) {
            layer->OnDetach();
        }
    }    

    void Scene::OnEvent(Event* e) {

        if (e->type == EVENT_TYPE_KEYPRESS) {
            if (e->key == GLFW_KEY_W) {
                activeCamera->moveForward(0.1);
            }
            if (e->key == GLFW_KEY_S) {
                activeCamera->moveForward(-0.1);
            }
            if (e->key == GLFW_KEY_A) {
                activeCamera->translate(0.1, 0.0, 0.0);
            }
            if (e->key == GLFW_KEY_D) {
                activeCamera->translate(-0.1, 0.0, 0.0);
            }
            if (e->key == GLFW_KEY_Q) {
                activeCamera->translate(-0.1, 0.0, 0.0);
            }
        }
        //pass the event to layerStack
        layerStack->OnEvent(e);

    }

    void Scene::OnDrawGUI(RenderParams* rp) {
        if(!enabled) 
            return;

        for(Layer* layer : layerStack->layers) {
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
        for(Layer* layer : layerStack->layers) {
            if(layer->enabled) {
                layer->setCamera( activeCamera );
                layer->OnUpdate(rp);
                layer->OnDraw(rp);
            }
        }        
    }

    void Scene::PushLayer(Layer* layer) {
        layer->setCamera(activeCamera);
        layerStack->PushLayer(layer);
    }

    LayerStack* Scene::GetLayerStack() {
        return layerStack;
    }

    void Scene::setActiveCamera(Camera* cam) {
        activeCamera = cam;
    }

    void Scene::handleResize(int width, int height) {
        activeCamera->handleResize(width, height);
    }

}