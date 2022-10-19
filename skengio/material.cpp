#include "material.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "defines.h"

namespace SKEngio {

    Material::Material() {

    }

    Material::~Material() {

    }

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

    glm::vec3 Material::GetAmbient() {
        return materialAmbientColor;
    }

    void Material::SetReflectivity(float _refl) {
        materialReflectivity = _refl;
    }

    glm::vec3 Material::GetDiffuse() {
        return materialDiffuseColor;
    }

    glm::vec3 Material::GetSpecular() {
        return materialSpecularColor;
    }

    float Material::GetShininess() {
        return materialShininess;
    }

    float Material::GetReflectivity() {
        return materialReflectivity;
    }

}