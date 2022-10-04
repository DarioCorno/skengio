#include "material.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "defines.h"

namespace SKEngio {
    Material::Material() {

    }

    Material::~Material() {

    }

    void Material::SetAmbient(float r, float g, float b, float a) {
        materialAmbientColor = glm::vec4( r, g, b, a);
    }
    
    void Material::SetDiffuse(float r, float g, float b, float a) {
        materialDiffuseColor = glm::vec4( r, g, b, a);
    }

    void Material::SetSpecular(float r, float g, float b, float a) {
        materialSpecularColor = glm::vec4( r, g, b, a);
    }

    void Material::SetShininess(float shiny) {
        materialShininess = shiny;
    }

}