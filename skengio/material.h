#pragma once

#ifndef _SK_MATERIAL_
#define _SK_MATERIAL_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace SKEngio  {

    class Material {
        public:

        glm::vec4 materialAmbientColor  = glm::vec4( 0.5f, 0.5f, 0.5f, 1 );
        glm::vec4 materialDiffuseColor  = glm::vec4( 0.7f, 0.7f, 0.7f, 1 );
        glm::vec4 materialSpecularColor = glm::vec4( 0.4f, 0.4f, 0.4f, 1 );
        float materialShininess = 1.0f;

        Material();

        ~Material();       

        void SetAmbient(float r, float g, float b, float a);
        void SetDiffuse(float r, float g, float b, float a);
        void SetSpecular(float r, float g, float b, float a);
        void SetShininess(float _shiny);

    };



}

#endif