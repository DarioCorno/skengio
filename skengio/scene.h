#pragma once

#include "defines.h"
#include "audiosource.h"
#include "layerstack.h"
#include "camera.h"

#include <string>
#include "renderParams.h"

namespace SKEngio {

    class Scene {
    public:

        unsigned int sceneID;

        float startTime = SCENE_DEFAULT_START;
        float endTime = SCENE_DEFAULT_END;
        float speedMult = 1;

        SKEngio::AudioSource* music{};

        Scene(unsigned int sceneid);

        ~Scene();

        void SetName(std::string n) { dispName = std::move(n); }

        void OnAttach();
        void OnDetach();

        void OnEvent(Event* e);

        void OnDrawGUI(RenderParams* rp);

        void UpdateAndDraw(RenderParams* rp);

        void PushLayer(Layer* layer);

        const LayerStack& GetLayerStack() const;

        void setActiveCamera(Camera* cam);

        void handleResize(int width, int height);

        std::string dispName;
        bool enabled = true;

    private:

        LayerStack layerStack;

        Camera* activeCamera;
        

    };

}
