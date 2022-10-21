#include "material.h"

namespace SKEngio {

    void Material::SetAmbient(float r, float g, float b) {
        materialAmbientColor = glm::vec3( r, g, b);
    }
    
    void Material::SetDiffuse(float r, float g, float b) {
        materialDiffuseColor = glm::vec3( r, g, b);
    }

    void Material::SetSpecular(float r, float g, float b) {
        materialSpecularColor = glm::vec3( r, g, b);
    }

    void Material::SetShininess(float shiny) {
        materialShininess = shiny;
    }

    const glm::vec3& Material::GetAmbient() const {
        return materialAmbientColor;
    }

    void Material::SetReflectivity(float _refl) {
        materialReflectivity = _refl;
    }

    const glm::vec3& Material::GetDiffuse() const {
        return materialDiffuseColor;
    }

    const glm::vec3& Material::GetSpecular() const {
        return materialSpecularColor;
    }

    float Material::GetShininess() const {
        return materialShininess;
    }

    float Material::GetReflectivity() const {
        return materialReflectivity;
    }

}