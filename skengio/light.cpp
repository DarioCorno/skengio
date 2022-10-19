#include "light.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "defines.h"

namespace SKEngio {

    Light::Light() {
        lightPosition = glm::vec3(0.0f, 14.0f, 0.0f );
        lightDiffuseColor = glm::vec3( 0.71f, 0.65f, 0.55f );
    }

    Light::~Light() {

    }

    void Light::SetPosition(float x, float y, float z) {
        lightPosition = glm::vec3( x, y, z ) ;
    }

    void Light::SetDiffuse(float r, float g, float b ) {
        lightDiffuseColor = glm::vec3( r, g, b );
    }

    glm::vec3 Light::GetPosition() {
        return lightPosition;
    }

    glm::vec3 Light::GetDiffuse() {
        return lightDiffuseColor;
    }

}