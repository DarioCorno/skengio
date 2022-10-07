#pragma once

#ifndef _SK_LIGHT_
#define _SK_LIGHT_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace SKEngio  {
    class Light {
        public:

        glm::vec3 lightPosition;
        glm::vec3 lightAmbientColor;
        glm::vec3 lightDiffuseColor;
        glm::vec3 lightSpecularColor;

        Light();

        ~Light();       

        void SetPosition(float x, float y, float z);
        void SetAmbient(float r, float g, float b);
        void SetDiffuse(float r, float g, float b);
        void SetSpecular(float r, float g, float b);

        glm::vec3 GetPosition();
        glm::vec3 GetAmbient();
        glm::vec3 GetDiffuse();
        glm::vec3 GetSpecular();

    };



}

#endif