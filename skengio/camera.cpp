#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>

#include "camera.h"

namespace SKEngio {

    Camera::Camera(unsigned int w, unsigned int h, float fov, unsigned int camID) {
        id = camID;
        width = w;
        height = h;
        fieldOfView = fov;
        nearPlane = 0.01f;
        farPlane = 100.0f;
        position = glm::vec3(0.0f, 0.0f , 3.0f);
        target = glm::vec3(0.0f, 0.0f, 0.0f);
        upVector = glm::vec3(0.0f, 1.0f, 0.0f);
        handleResize(w,h);
    };

    Camera::~Camera() {

    }

    void Camera::setPosition(glm::vec3 newPos) {
        position = newPos;
    }

    void Camera::setPosition(float x, float y, float z) {
        position = glm::vec3( x, y, z );
    }

    void Camera::setTarget(float x, float y, float z) {
        target = glm::vec3(x, y, z);
    }

    void Camera::setTarget(glm::vec3 newTarget) {
        target = newTarget;
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt( position, target, upVector );
    }

    glm::mat4 Camera::getProjMatrix() {
        return glm::perspective(fieldOfView, (float)width / (float)height, nearPlane, farPlane);
    }

    glm::vec3 Camera::getDirection() {
        return glm::normalize(target - position);
    }

    void Camera::moveForward(float distance) {
        glm::vec3 dir = getDirection();
        target = target + (dir * distance);
        position = position + (dir * distance);
    }

    void Camera::translate(float x, float y, float z) {
        position += glm::vec3( x,y,z);
    }

    void Camera::handleResize(unsigned int w, unsigned int h) {
		if(h == 0) h = 1;
        
        width = w;
        height = h;

		float ratio = w / float(h);
		glViewport(0, 0, w, h);

		projMatrix = glm::perspective(fieldOfView, ratio, nearPlane, farPlane);    
		
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //setPerspective(fovAngle, ratio, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);            
        std::cout << "Camera " << id << " resize to: " << width << " " << height << std::endl;
    }

}