#include "light.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "defines.h"

namespace SKEngio {
    Light::Light() {
        lightPosition = glm::vec3(4.0f, 4.0f, -4.0f );
        lightAmbientColor = glm::vec3( 0.5f, 0.2f, 0.8f );
        lightDiffuseColor = glm::vec3( 1.0f, 0.7f, 0.9f );
        lightSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f );
    }

    Light::~Light() {

    }

    void Light::SetPosition(float x, float y, float z) {
        lightPosition = glm::vec3( x, y, z ) ;
    }

    void Light::SetAmbient(float r, float g, float b ) {
        lightAmbientColor = glm::vec3( r, g, b );
    }
    
    void Light::SetDiffuse(float r, float g, float b ) {
        lightDiffuseColor = glm::vec3( r, g, b );
    }

    void Light::SetSpecular(float r, float g, float b ) {
        lightSpecularColor = glm::vec3( r, g, b );
    }

    glm::vec3 Light::GetPosition() {
        return lightPosition;
    };

    glm::vec3 Light::GetAmbient() {
        return lightAmbientColor;
    };

    glm::vec3 Light::GetDiffuse() {
        return lightDiffuseColor;
    };

    glm::vec3 Light::GetSpecular() {
        return lightSpecularColor;
    };

}