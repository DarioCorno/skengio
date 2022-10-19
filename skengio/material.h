#pragma once

#ifndef _SK_MATERIAL_
#define _SK_MATERIAL_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils/texture.h"

namespace SKEngio  {

    class Texture;

    class Material {
        public:

        glm::vec3 materialAmbientColor  = glm::vec3( 0.5f, 0.5f, 0.5f );
        glm::vec3 materialDiffuseColor  = glm::vec3( 0.7f, 0.7f, 0.7f );
        glm::vec3 materialSpecularColor = glm::vec3( 1.0f, 1.0f, 1.0f );
        float materialShininess = 32.0f;
        float materialReflectivity = 0.4f;

        SKEngio::Texture* diffuseTexture;

        Material();

        ~Material();       

        void SetAmbient(float r, float g, float b);
        void SetDiffuse(float r, float g, float b);
        void SetSpecular(float r, float g, float b);
        void SetShininess(float _shiny);
        void SetReflectivity(float _refl);

        glm::vec3 GetAmbient();
        glm::vec3 GetDiffuse();
        glm::vec3 GetSpecular();
        float GetShininess();
        float GetReflectivity();


    };



}

#endif