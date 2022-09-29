#pragma once

#ifndef _SK_LIGHT_
#define _SK_LIGHT_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace SKEngio  {
    class Light {
        public:

        glm::vec4 lightPosition = glm::vec4( 0, 0, 1, 0 );
        glm::vec4 lightAmbientColor  = glm::vec4( 0.3f, 0.3f, 0.3f, 1 );
        glm::vec4 lightDiffuseColor  = glm::vec4( 0.7f, 0.7f, 0.7f, 1 );
        glm::vec4 lightSpecularColor = glm::vec4( 1.0f, 1.0f, 1.0f, 1 );

        Light();

        ~Light();       

        void SetPosition(float x, float y, float z);
        void SetAmbient(float r, float g, float b, float a);
        void SetDiffuse(float r, float g, float b, float a);
        void SetSpecular(float r, float g, float b, float a);

    };



}

#endif