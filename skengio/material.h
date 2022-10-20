#pragma once

#ifndef SK_MATERIAL_
#define SK_MATERIAL_

#include "glm/glm.hpp"

#include "utils/texture.h"

namespace SKEngio  {
    class Material {
    public:

        glm::vec3 materialAmbientColor{ 0.5f, 0.5f, 0.5f };
        glm::vec3 materialDiffuseColor{ 0.7f, 0.7f, 0.7f };
        glm::vec3 materialSpecularColor{ 1.0f, 1.0f, 1.0f };
        float materialShininess = 32.0f;
        float materialReflectivity = 0.4f;

        Texture* diffuseTexture;

        void SetAmbient(float r, float g, float b);
        void SetDiffuse(float r, float g, float b);
        void SetSpecular(float r, float g, float b);
        void SetShininess(float _shiny);
        void SetReflectivity(float _refl);

        const glm::vec3& GetAmbient() const;
        const glm::vec3& GetDiffuse() const;
        const glm::vec3& GetSpecular() const;
        float GetShininess() const;
        float GetReflectivity() const;


    };



}

#endif