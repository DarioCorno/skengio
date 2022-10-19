#pragma once

#ifndef SK_CAMERA_
#define SK_CAMERA_

#include "glm/glm.hpp"

#include <string>

namespace SKEngio {

    class Camera {
        public:

            std::string id;
            glm::vec3 position;
            glm::vec3 target;
            glm::vec3 upVector;

            float fieldOfView = 45.0f;
            unsigned int width;
            unsigned int height;
            float nearPlane;
            float farPlane;

            glm::mat4 viewMatrix;
            glm::mat4 projMatrix;

            Camera(unsigned int w, unsigned int h, float fov, std::string camID);

            ~Camera() = default;
    
            void setTarget(float x, float y, float z);
            void setTarget(const glm::vec3& newTarget);
            void translate(float x, float y, float z);

            void setPosition(float x, float y, float z);
            void setPosition(const glm::vec3& newPos);

            void moveForward(float distance);

            glm::mat4 getViewMatrix() const;
            glm::mat4 getProjMatrix() const;

            glm::vec3 getDirection() const;

            void handleResize(unsigned int w, unsigned int h);
            void UpdateViewport();
    };
}

#endif
