#include "scene.h"

namespace SKEngio {

    Scene::Scene(unsigned int sceneid) {
        //initializes the layers manager
        layerStack = new LayerStack();
        sceneID = sceneid;
    };

    Scene::~Scene() {

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

        //pass the event to layerStack
        layerStack->OnEvent(e);

    }

    void Scene::OnDrawGUI(float timeValue) {
        if(!enabled) return;

        for(Layer* layer : layerStack->layers) {
            if(layer->enabled) {
                layer->OnDrawGUI(timeValue);
            }
        }
    }

    void Scene::UpdateAndDraw(float timeValue) {
        
        if(!enabled) return;

        //update and render all scenes
        for(Layer* layer : layerStack->layers) {
            if(layer->enabled) {
                layer->setCamera( activeCamera );
                layer->OnUpdate(timeValue);
                layer->OnDraw(0.0f);
            }
        }        
    }

    LayerStack* Scene::GetLayerStack() {
        return layerStack;
    }

    void Scene::Destroy() {
        layerStack->Destroy();
    }

    void Scene::addCamera(Camera* newCamera) {
        cameraList.push_back(newCamera);
    }

    void Scene::setActiveCamera(unsigned int camID) {
        for(Camera* cam : cameraList) {
            if (cam->id == camID) {
                activeCamera = cam;
                break;
            }
        }   
    }

    void Scene::handleResize(int width, int height) {
        activeCamera->handleResize(width, height);
    }

}