#pragma once

#include "defines.h"

#include "layerstack.h"
#include "camera.h"

namespace SKEngio {

    class Scene {
        public:

        unsigned int sceneID;

        float startTime = SCENE_DEFAULT_START;
        float endTime = SCENE_DEFAULT_END;
        float speedMult = 1;

        Scene(unsigned int sceneid);

        ~Scene();

        void OnAttach();
        void OnDetach();

        void OnEvent(Event* e);

        void OnDrawGUI(float timeValue);

        void UpdateAndDraw(float timeValue);

        LayerStack* GetLayerStack();
        void Destroy();

        void addCamera(Camera* newCam);
        void setActiveCamera(unsigned int camID);

        void handleResize(int width, int height);

        private:

        LayerStack* layerStack;

        std::vector<Camera*> cameraList;
        Camera* activeCamera;
        

    };

}