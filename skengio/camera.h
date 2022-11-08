#pragma once

#ifndef SK_CAMERA_
#define SK_CAMERA_

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include <string>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

namespace SKEngio {

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Camera {
        public:

            std::string id;

            glm::vec3 position;
            glm::vec3 target;
            glm::vec3 upVector;
            glm::vec3 rightVector;

            float fieldOfView = 45.0f;
            unsigned int width;
            unsigned int height;
            float nearPlane;
            float farPlane;

            glm::mat4 viewMatrix;
            glm::mat4 projMatrix;

            Camera(unsigned int w, unsigned int h, float fov, std::string camID);

            void setTarget(float x, float y, float z);
            void setTarget(const glm::vec3& newTarget);
            void translate(float x, float y, float z);

            void setPosition(float x, float y, float z);
            void setPosition(const glm::vec3& newPos);

            void moveForward(float distance);

            glm::mat4 getViewMatrix();
            glm::mat4 getProjMatrix();

            glm::vec3 getDirection() const;

            void handleResize(unsigned int w, unsigned int h);
            void UpdateViewport();

        private:
            bool isDirty = true;
            void UpdateUpVector();

    };
}

#endif
