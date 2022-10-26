#include "light.h"

#include "skengio/entities/geometries/box.h"

namespace SKEngio {

    void Light::SetPosition(float x, float y, float z) {
        transform.setLocalPosition( glm::vec3( x, y, z ) );
    }

    void Light::SetDiffuse(float r, float g, float b ) {
        lightDiffuseColor = glm::vec3( r, g, b );
    }

    glm::vec3 Light::GetPosition() {
        return transform.getGlobalPosition();
    }

    glm::vec3 Light::GetDiffuse() {
        return lightDiffuseColor;
    }

    glm::mat4 Light::getLightViewProjMatrix() {
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 30.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt( transform.getGlobalPosition(), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        return lightSpaceMatrix;
    }


}