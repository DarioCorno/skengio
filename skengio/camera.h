#pragma once

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace SKEngio {

    class Camera {
        public:

        unsigned int id;
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

        Camera(unsigned int w, unsigned int h, float fov, unsigned int camID);

        ~Camera();
    
        void setTarget(float x, float y, float z);
        void setTarget(glm::vec3 newTarget);
        void translate(float x, float y, float z);

        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3 newPos);

        void moveForward(float distance);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjMatrix();

        glm::vec3 getDirection();

        void handleResize(unsigned int w, unsigned int h);

    };
}