#pragma once

#include "defines.h"
#include "audiosource.h"
#include "camera.h"
#include "event.h"

#include <string>
#include "renderParams.h"
#include "skengio/entities/light.h"
#include "skengio/layer.h"
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace SKEngio {

    class Scene {
    public:

        unsigned int sceneID;

        float startTime = SCENE_DEFAULT_START;
        float endTime = SCENE_DEFAULT_END;
        float speedMult = 1;

        SKEngio::AudioSource* music{};

        Scene() = default;

        // prevent copying object
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;

        virtual ~Scene() = default;

        void SetName(std::string n) { dispName = std::move(n); }
        std::string GetName();

        virtual void OnAttach();
        virtual void OnDetach();
        virtual void OnEvent(Event* e);
        virtual void OnDrawGUI(RenderParams* rp);
        virtual void OnUpdate(RenderParams* rp);
        virtual void OnDraw(RenderParams* rp);

        //void PushLayer(Layer* layer);

        Camera* SetCamera(float fov, std::string camID);
        Light* NewLight();
        Entity* NewEntity();

        void handleResize(int width, int height);

        std::string dispName;
        bool enabled = true;

        Camera* camera = nullptr;
        std::vector<Light*> lights;
        std::vector<Entity*> entities;

    };

}
