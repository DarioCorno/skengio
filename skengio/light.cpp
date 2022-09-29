#include "light.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "defines.h"

namespace SKEngio {
    Light::Light() {

    }

    Light::~Light() {

    }

    void Light::SetPosition(float x, float y, float z) {
        lightPosition = glm::vec4( x, y, z, 1.0f) ;
    }

    void Light::SetAmbient(float r, float g, float b, float a) {
        lightAmbientColor = glm::vec4( r, g, b, a);
    }
    
    void Light::SetDiffuse(float r, float g, float b, float a) {
        lightDiffuseColor = glm::vec4( r, g, b, a);
    }

    void Light::SetSpecular(float r, float g, float b, float a) {
        lightSpecularColor = glm::vec4( r, g, b, a);
    }

}