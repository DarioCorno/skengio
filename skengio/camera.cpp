#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace SKEngio {

    Camera::Camera(unsigned int w, unsigned int h, float fov, std::string camID) :
        id{std::move(camID)},
        position{0.0f, 30.0f, 30.0f},
        target{0.0f, 0.0f, 0.0f},
        upVector{0.0f, 1.0f, 0.0f},
        fieldOfView{fov},
        width{w},
        height{h},
        nearPlane{0.01f},
        farPlane{100.0f}
    {
        handleResize(w,h);
    }

    void Camera::setPosition(const glm::vec3& newPos) {
        position = newPos;
        isDirty = true;
    }

    void Camera::setPosition(float x, float y, float z) {
        position = glm::vec3( x, y, z );
        isDirty = true;
    }

    void Camera::setTarget(float x, float y, float z) {
        target = glm::vec3(x, y, z);
        isDirty = true;
    }

    void Camera::setTarget(const glm::vec3& newTarget) {
        target = newTarget;
        isDirty = true;
    }

    //rotate around the world Y axis and world Z axis
    void Camera::rotate(const float hRotDelta, const float vRotDelta) {
        glm::vec3 direction(target - position);
        float distance = glm::length(direction);
        glm::normalize(direction);

        glm::mat4 rotMat(1.0f);

        rotMat = glm::rotate(rotMat, glm::degrees(hRotDelta / camSpeedMult), glm::vec3(0.0f, 1.0f, 0.0f));
        rotMat = glm::rotate(rotMat, glm::degrees(vRotDelta / camSpeedMult), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec3 newDir = glm::normalize( glm::vec3(rotMat * glm::vec4(direction,1.0f) ) );
        newDir *= distance;

        target = position + newDir;

        UpdateUpRightVector();

        isDirty = true;
    }

    void Camera::strafe(const float xDelta) {
        position += (rightVector * (xDelta / (camSpeedMult / 2.0f)));
        target += (rightVector * (xDelta / (camSpeedMult / 2.0f)));
        isDirty = true;
    }

    void Camera::rise(const float yDelta) {
        position += (upVector * (yDelta / (camSpeedMult / 2.0f)));
        target += (upVector * (yDelta / (camSpeedMult / 2.0f)));
        isDirty = true;
    }


    glm::mat4 Camera::getViewMatrix() {
        if (isDirty) {
            UpdateUpRightVector();
            viewMatrix = glm::lookAt(position, target, upVector);
        }

        return viewMatrix;
    }

    void Camera::UpdateUpRightVector() {
        glm::vec3 direction = glm::normalize(target - position);
        glm::vec3 right = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));

        upVector = glm::normalize( glm::cross(right, direction) );
        rightVector = glm::normalize(right);
    }

    glm::mat4 Camera::getProjMatrix() {
        return projMatrix; // = glm::perspective(fieldOfView, (float)width / (float)height, nearPlane, farPlane);

    }

    glm::vec3 Camera::getDirection() const {
        return glm::normalize(target - position);
    }

    void Camera::moveForward(float distance) {
        glm::vec3 dir = getDirection();
        target = target + (dir * distance);
        position = position + (dir * distance);
        isDirty = true;
    }

    void Camera::translate(float x, float y, float z) {
        position += glm::vec3( x,y,z);
        target += glm::vec3(x, y, z);
        isDirty = true;
    }

    void Camera::handleResize(unsigned int w, unsigned int h) {
		if(h == 0) h = 1;
        
        width = w;
        height = h;

        UpdateViewport();
    }

    void Camera::UpdateViewport() {

        float ratio = width / float(height);
        glViewport(0, 0, width, height);

        projMatrix = glm::perspective(fieldOfView, ratio, nearPlane, farPlane);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //setPerspective(fovAngle, ratio, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

}
