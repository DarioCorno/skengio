#pragma once

#ifndef SK_LIGHT_
#define SK_LIGHT_

#include "glm/glm.hpp"
#include "skengio/utils/geometries/box.h"
#include "skengio/camera.h"

#include <memory>

namespace SKEngio  {

    class Object;

    class Light {
        public:

            glm::vec3 lightPosition{ 0.0f, 14.0f, 0.0f };
            glm::vec3 lightDiffuseColor{ 0.71f, 0.65f, 0.55f };

            void SetPosition(float x, float y, float z);
            void SetDiffuse(float r, float g, float b);

            glm::vec3 GetPosition();
            glm::vec3 GetDiffuse();

            void enableDebug();
            void setDebugCamera(Camera* cam);
            void drawDebug();

        private:
            std::unique_ptr<Object> lightdeb;
            bool hasDebug = false;

    };



}

#endif