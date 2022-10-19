#include "light.h"

namespace SKEngio {
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